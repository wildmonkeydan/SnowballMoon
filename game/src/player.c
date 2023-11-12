#include <raylib.h>
#include <raymath.h>
#include "player.h"
#include "input.h"
#include "snowball.h"

#define PLAYER_SPEED 30.0f
#define MAX_SNOWBALLS 256

#define NORM_COLLISION (Rectangle) { 0, 0, 64, 128}
#define CROUCH_COLLISION (Rectangle) { 0, -64, 64, 64}
#define NO_COLLISION (Rectangle) { 0, 0, 0, 0};

void CreatePlayer(Player* player, int id) {
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

int UpdatePlayer(Player* player, float delta, Snowball* sb, int nextSnowball) {
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
			CreateSnowballStraight(&sb[nextSnowball], player->id, player->angle, player->flipped ? -1 : 1);
			nextSnowball++;

			if (nextSnowball >= MAX_SNOWBALLS) {
				nextSnowball = 0;
			}

			player->hasSnowball = false;
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
				CreateSnowballStraight(&sb[nextSnowball], player->id, player->angle, player->flipped ? -1 : 1);
				nextSnowball++;

				if (nextSnowball >= MAX_SNOWBALLS) {
					nextSnowball = 0;
				}

				player->hasSnowball = false;
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
			if (player->stateTimer > 0.5f) {
				player->hasSnowball = true;
			}
		}
		break;
	case PS_THROW:
		player->collision = NO_COLLISION;

		if (player->stateTimer >= 0.2f) {
			player->collision = NORM_COLLISION;
			player->state = PS_STAND;
		}
		break;
	case PS_HIT:
		if (player->stateTimer >= 0.5f) {
			player->state = PS_STAND;
		}
		break;
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

void DrawPlayer(Player* player, Vector2 moonMiddle, float moonRadius, Texture2D playerTex, float delta) {

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
	}

	Rectangle texRect = { uv.x + 1, uv.y, player->flipped ? -128 : 128, 127 };
	Rectangle rect = { x + moonMiddle.x, y + moonMiddle.y, 128, 128 };

	DrawTexturePro(playerTex, texRect, rect, (Vector2) { 64, 128 }, player->angle + 90, player->colour);
}