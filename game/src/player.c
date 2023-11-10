#include <raylib.h>
#include <raymath.h>
#include "player.h"
#include "input.h"

#define PLAYER_SPEED 20.0f

void CreatePlayer(Player* player, int id) {
	player->angle = 0.f;
	player->colour = YELLOW;
	player->id = id;
	player->hasSnowball = false;
	player->state = PS_STAND;
	player->score = 0;
}

void UpdatePlayer(Player* player, float delta) {
	if (input_GetButton(GI_LEFT, player->id)) {
		player->angle -= PLAYER_SPEED * delta;
	}
	else if (input_GetButton(GI_RIGHT, player->id)) {
		player->angle += PLAYER_SPEED * delta;
	}
}

void DrawPlayer(Player* player, Vector2 moonMiddle, float moonRadius, Texture2D playerTex) {
	int x = moonRadius * cosf(DEG2RAD * player->angle);
	int y = moonRadius * sinf(DEG2RAD * player->angle);

	Rectangle texRect = { 0, 0, playerTex.width, playerTex.height };
	Rectangle rect = { x + moonMiddle.x, y + moonMiddle.y, playerTex.width, playerTex.height };

	DrawTexturePro(playerTex, texRect, rect, (Vector2) { playerTex.width / 2, playerTex.height }, player->angle + 90, player->colour);
}