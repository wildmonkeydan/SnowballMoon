#pragma once
#include <raylib.h>
#include "animation.h"
#include "types.h"


void CreatePlayer(Player* player, int id);
int UpdatePlayer(Player* player, float delta, Snowball* sb, int nextSnowball);
void DrawPlayer(Player* player, Vector2 moonMiddle, float moonRadius, Texture2D playerTex, float delta);