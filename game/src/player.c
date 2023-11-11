#include <raylib.h>
#include <raymath.h>
#include "player.h"
#include "input.h"

#define PLAYER_SPEED 30.0f

void CreatePlayer(Player* player, int id) {
	player->angle = 0.f;
	player->colour = YELLOW;
	player->id = id;
	player->hasSnowball = false;
	player->flipped = false;
	animation_ClearContext(&player->ctx);
	player->state = PS_STAND;
	player->score = 0;
}

void UpdatePlayer(Player* player, float delta) {
	player->ctx.loop = true;

	if (input_GetButton(GI_LEFT, player->id) || input_GetButton(GI_RIGHT, player->id) || input_GetButton(GI_DOWN, player->id)) {
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
	}
	else {
		player->state = PS_STAND;		
	}
	
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