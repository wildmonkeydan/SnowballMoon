#pragma once
#include <raylib.h>
#include "animation.h"

typedef enum {
	PS_STAND,
	PS_CROUCH,
	PS_COLLECT,
	PS_THROW,
	PS_RUN
}PlayerState;

typedef struct {
	float angle;
	PlayerState state;
	Color colour;
	bool hasSnowball;
	bool flipped;
	int score;
	int id;
	AnimContext ctx;
}Player;

void CreatePlayer(Player* player, int id);
void UpdatePlayer(Player* player, float delta);
void DrawPlayer(Player* player, Vector2 moonMiddle, float moonRadius, Texture2D playerTex, float delta);