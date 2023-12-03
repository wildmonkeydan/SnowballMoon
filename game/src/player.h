#pragma once
#include <raylib.h>
#include "animation.h"
#include "types.h"


void CreatePlayer(Player* player, int playerSize, int id, int playerId);
int UpdatePlayer(Player* player, int playerSize, float delta, Snowball* sb, int nextSnowball, Vector2 moonMiddle, float moonRadius);
void DrawPlayer(Player* player, Vector2 moonMiddle, float moonRadius, Texture2D playerTex, int playerSize, float delta);