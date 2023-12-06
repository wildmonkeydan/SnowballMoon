#include <raylib.h>
#include <raymath.h>
#include "player.h"
#include "input.h"
#include "snowball.h"

#define PLAYER_SPEED 25.0f
#define PLAYER_STEP 0.025f
#define PLAYER_THROW_STRENGTH 50
#define MAX_SNOWBALLS 16

#define NORM_COLLISION (Rectangle){ 0, 0, playerSize / 2, playerSize }
#define CROUCH_COLLISION (Rectangle) { 0, -playerSize / 2, playerSize / 2, playerSize / 2}
#define NO_COLLISION (Rectangle) { 0, 0, 0, 0};
#define COLLISION_DEBUG

void CreatePlayer(Player* player, int playerSize, int id, int playerId) {
	player->angle = 0.f;
	player->colour = YELLOW;
	player->id = id;
	player->playerId = playerId;
	player->hasSnowball = false;
	player->flipped = false;
	player->throwing = false;
	player->snowballAngle = 0.f;
	animation_ClearContext(&player->ctx);
	player->state = PS_STAND;
	player->stateTimer = 0;
	player->collision = NORM_COLLISION;
	player->score = 0;
}

int UpdatePlayer(Player* player, int playerSize, float delta, Snowball* sb, int nextSnowball, Vector2 moonMiddle, float moonRadius) {
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
		}
		if (input_GetButtonPressed(GI_ATTACK, player->id) && player->hasSnowball) {
			player->stateTimer = 0;

			player->throwing = true;
		}
		if (player->hasSnowball && player->stateTimer > 0.25f && player->throwing) {
			if (input_GetButton(GI_ATTACK, player->id)) {
				player->state = PS_AIM;
				player->snowballAngle = 0;
			}
			else {
				player->state = PS_THROW;
			}
			player->throwing = false;
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
			}
			if (input_GetButtonPressed(GI_ATTACK, player->id) && player->hasSnowball) {
				player->stateTimer = 0;
			}
			if (input_GetButton(GI_ATTACK, player->id) && player->hasSnowball && player->stateTimer > 2.f) {
				player->state = PS_THROW;
			}
		}
		break;
	case PS_CROUCH:
		player->ctx.loop = false;
		

		if (!input_GetButton(GI_DOWN, player->id)) {
			player->state = PS_STAND;
		}

		if (player->stateTimer > 0.5f) {
			player->collision = CROUCH_COLLISION;
			if (input_GetButtonPressed(GI_ATTACK, player->id)) {
				player->state = PS_COLLECT;
			}
		}

		break;
	case PS_THROW:
		player->ctx.loop = false;

		if (player->stateTimer >= 0.8f && player->stateTimer < 0.9f) {
			player->angle += (PLAYER_STEP) * player->flipped ? -1 : 1;
		}

		if (player->stateTimer >= 1.1f && player->hasSnowball) {
			CreateSnowballStraight(&sb[nextSnowball], playerSize, player->playerId, player->angle + (player->flipped ? -5.f : 5.f), player->flipped ? -1 : 1);
			
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
		break;
	case PS_AIM:
		player->ctx.loop = false;

		if (player->stateTimer <= 1.f) {
			player->snowballAngle += 90.f * delta;
		}
		else if(player->stateTimer > 1.f) {
			player->snowballAngle -= 90.f * delta;
			if (player->snowballAngle <= 0.f) {
				player->stateTimer = 0;
			}
		}

		if (!input_GetButton(GI_ATTACK, player->id)) {
			CreateSnowballGravity(&sb[nextSnowball], player->playerId, player->angle + (player->flipped ? -5.f : 5.f), moonMiddle, moonRadius, Vector2Rotate((Vector2) { (player->flipped ? -1200.f : 1200.f), player->snowballAngle * 5.F }, (player->angle + 90)* DEG2RAD), playerSize);

			nextSnowball++;

			if (nextSnowball >= MAX_SNOWBALLS) {
				nextSnowball = 0;
			}

			player->hasSnowball = false;

			player->state = PS_STAND;
		}
		break;
	}

	if (player->prevState != player->state) {
		player->stateTimer = 0;
		animation_ClearContext(&player->ctx);
	}
	
	// Rotate collision

	int x = (moonRadius + player->collision.y) * cosf(DEG2RAD * player->angle);
	int y = (moonRadius + player->collision.y) * sinf(DEG2RAD * player->angle);

	float sinRotation = sinf((player->angle + 90) * DEG2RAD);
	float cosRotation = cosf((player->angle + 90) * DEG2RAD);
	float dx = -playerSize / 4;
	float dy = -playerSize;

	player->collisionPoly[3].x = x + dx * cosRotation - dy * sinRotation;
	player->collisionPoly[3].y = y + dx * sinRotation + dy * cosRotation;

	player->collisionPoly[2].x = x + dx * cosRotation - (dy + player->collision.height) * sinRotation;
	player->collisionPoly[2].y = y + dx * sinRotation + (dy + player->collision.height) * cosRotation;


	player->collisionPoly[1].x = x + (dx + player->collision.width) * cosRotation - (dy + player->collision.height) * sinRotation;
	player->collisionPoly[1].y = y + (dx + player->collision.width) * sinRotation + (dy + player->collision.height) * cosRotation;


	player->collisionPoly[0].x = x + (dx + player->collision.width) * cosRotation - dy * sinRotation;
	player->collisionPoly[0].y = y + (dx + player->collision.width) * sinRotation + dy * cosRotation;

	for (int i = 0; i < 4; i++) {
		player->collisionPoly[i] = Vector2Add(player->collisionPoly[i], moonMiddle);
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

void DrawPlayer(Player* player, Vector2 moonMiddle, float moonRadius, Texture2D playerTex, Texture2D arrowTex, int playerSize, float delta) {

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

	if (player->state == PS_AIM) {
		int arrowX = (moonRadius + (playerSize / 2)) * cosf(DEG2RAD * (player->angle + (player->flipped ? -5.f : 5.f)));
		int arrowY = (moonRadius + (playerSize / 2)) * sinf(DEG2RAD * (player->angle + (player->flipped ? -5.f : 5.f)));

		Rectangle arrowTexRect = { 0, 0, arrowTex.width, arrowTex.height };
		Rectangle arrowRect = { arrowX + moonMiddle.x, arrowY + moonMiddle.y, playerSize / 2, playerSize / 2 };

		DrawTexturePro(arrowTex, arrowTexRect, arrowRect, (Vector2) { 0, playerSize / 4 }, player->snowballAngle + (player->flipped ? player->angle - 90.f : player->angle), RAYWHITE);
	}

	#ifdef COLLISION_DEBUG

	DrawLineV(player->collisionPoly[0], player->collisionPoly[1], GREEN);
	DrawLineV(player->collisionPoly[1], player->collisionPoly[2], GREEN);
	DrawLineV(player->collisionPoly[2], player->collisionPoly[3], GREEN);
	DrawLineV(player->collisionPoly[3], player->collisionPoly[0], GREEN);

	DrawTriangle(player->collisionPoly[1], player->collisionPoly[0], player->collisionPoly[3], ColorAlpha(GREEN, 0.5f));
	DrawTriangle(player->collisionPoly[3], player->collisionPoly[2], player->collisionPoly[1], ColorAlpha(DARKGREEN, 0.5f));

	#endif // COLLISION_DEBUG

}