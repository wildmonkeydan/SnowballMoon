#include <raylib.h>
#include <raymath.h>
#include "player.h"
#include "input.h"
#include "snowball.h"

#define PLAYER_SPEED 25.0f
#define PLAYER_STEP 1.5f
#define MAX_SNOWBALLS 256

#define NORM_COLLISION (Rectangle){ 0, 0, playerSize / 2, playerSize }
#define CROUCH_COLLISION (Rectangle) { 0, playerSize * 0.75f, playerSize / 4, playerSize / 2}
#define NO_COLLISION (Rectangle) { 0, 0, 0, 0};

void CreatePlayer(Player* player, int playerSize, int id) {
	player->angle = 0.f;
	player->colour = YELLOW;
	player->id = id;
	player->hasSnowball = false;
	player->flipped = false;
	animation_ClearContext(&player->ctx);
	player->state = PS_STAND;
	player->stateTimer = 0;
	player->collision = NORM_COLLISION;
	player->score = 0;
}

int UpdatePlayer(Player* player, int playerSize, float delta, Snowball* sb, int nextSnowball) {
	player->ctx.loop = true;
	player->stateTimer += delta;
	player->collision = NORM_COLLISION;

	switch (player->state) {
	case PS_STAND:
		if (input_GetButton(GI_LEFT, player->id)) {
			player->angle -= PLAYER_SPEED * delta;
			player->flipped = true;
			player->state = PS_RUN;
		}
		else if (input_GetButton(GI_RIGHT, player->id)) {
			player->angle += PLAYER_SPEED * delta;
			player->flipped = false;
			player->state = PS_RUN;
		}
		else if (input_GetButton(GI_DOWN, player->id)) {
			player->state = PS_CROUCH;
			player->ctx.loop = false;
			player->collision = CROUCH_COLLISION;
		}
		else if (input_GetButtonPressed(GI_ATTACK, player->id) && player->hasSnowball) {
			player->state = PS_THROW;
			player->collision = NO_COLLISION;
		}

		break;
	case PS_RUN:
		if (!(input_GetButton(GI_LEFT, player->id) || input_GetButton(GI_RIGHT, player->id))) {
			player->state = PS_STAND;
		}
		else {
			if (input_GetButton(GI_LEFT, player->id)) {
				player->angle -= PLAYER_SPEED * delta;
				player->flipped = true;
			}
			else if (input_GetButton(GI_RIGHT, player->id)) {
				player->angle += PLAYER_SPEED * delta;
				player->flipped = false;
			}
			else if (input_GetButton(GI_DOWN, player->id)) {
				player->state = PS_CROUCH;
				player->ctx.loop = false;
				player->collision = CROUCH_COLLISION;
			}
			if (input_GetButtonPressed(GI_ATTACK, player->id) && player->hasSnowball) {
				player->state = PS_THROW;
				player->collision = NO_COLLISION;
			}
		}
		break;
	case PS_CROUCH:
		player->ctx.loop = false;
		player->collision = CROUCH_COLLISION;

		if (!input_GetButton(GI_DOWN, player->id)) {
			player->state = PS_STAND;
		}
		else if(input_GetButtonPressed(GI_ATTACK, player->id)){
			player->state = PS_COLLECT;
		}
		break;
	case PS_THROW:
		player->collision = NO_COLLISION;
		player->ctx.loop = false;

		if (player->stateTimer >= 0.8f && player->stateTimer < 1.1f) {
			player->angle += (PLAYER_STEP * delta) * player->flipped ? -1 : 1;
		}

		if (player->stateTimer >= 1.1f && player->hasSnowball) {
			CreateSnowballStraight(&sb[nextSnowball], playerSize, player->id, player->angle, player->flipped ? -1 : 1);
			nextSnowball++;

			if (nextSnowball >= MAX_SNOWBALLS) {
				nextSnowball = 0;
			}

			player->hasSnowball = false;
		}

		if (player->stateTimer >= 1.5f) {
			player->collision = NORM_COLLISION;
			player->state = PS_STAND;
		}
		break;
	case PS_HIT:
		if (player->stateTimer >= 1.f) {
			player->state = PS_STAND;
		}
		break;
	case PS_COLLECT:
		player->ctx.loop = false;

		if (player->stateTimer >= 0.95f) {
			player->state = PS_CROUCH;
			player->hasSnowball = true;
		}
	}

	if (player->prevState != player->state) {
		player->stateTimer = 0;
		animation_ClearContext(&player->ctx);
	}

	if (player->angle >= 360) {
		player->angle = 0;
	}
	if (player->angle < 0) {
		player->angle = 359;
	}

	player->prevState = player->state;
	
	return nextSnowball;
}

void DrawPlayer(Player* player, Vector2 moonMiddle, float moonRadius, Texture2D playerTex, int playerSize, float delta) {

	int x = moonRadius * cosf(DEG2RAD * player->angle);
	int y = moonRadius * sinf(DEG2RAD * player->angle);

	Vector2 uv = { 0 };

	switch (player->state) {
	case PS_STAND:
		uv = animation_AnimateDef(DA_PLAYERIDLE, &player->ctx, delta);
		break;
	case PS_RUN:
		uv = animation_AnimateDef(DA_PLAYERRUN, &player->ctx, delta);
		break;
	case PS_CROUCH:
		uv = animation_AnimateDef(DA_PLAYERCROUCH, &player->ctx, delta);
		break;
	case PS_COLLECT:
		uv = animation_AnimateDef(DA_PICKUP, &player->ctx, delta);
		break;
	case PS_THROW:
		uv = animation_AnimateDef(DA_THROW, &player->ctx, delta);
		break;
	case PS_HIT:
		uv = animation_AnimateDef(DA_HITFRONT, &player->ctx, delta);
		break;
	}

	Rectangle texRect = { uv.x + 1, uv.y, player->flipped ? -128 : 128, 128 };
	Rectangle rect = { x + moonMiddle.x, y + moonMiddle.y, playerSize, playerSize };

	DrawTexturePro(playerTex, texRect, rect, (Vector2) { playerSize / 2, playerSize }, player->angle + 90, player->colour);
}