#pragma once
#include <raylib.h>

typedef enum {
	PS_STAND,
	PS_CROUCH,
	PS_COLLECT,
	PS_THROW
}PlayerState;

typedef struct {
	float angle;
	PlayerState state;
	Color colour;
	bool hasSnowball;
	int score;
	int id;
}Player;

void CreatePlayer(Player* player, int id);
void UpdatePlayer(Player* player, float delta);
void DrawPlayer(Player* player, Vector2 moonMiddle, float moonRadius, Texture2D playerTex);