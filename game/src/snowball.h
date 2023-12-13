#pragma once
#include <raylib.h>
#include "types.h"

void CreateSnowballStraight(Snowball* sb, int playerHeight, int owner, float angle, int direction);
void CreateSnowballGravity(Snowball* sb, int owner, float angle, Vector2 moonMiddle, float moonRadius, Vector2 direction, int playerHeight);
void UpdateSnowball(Snowball* sb, Player* players, Fort* forts, GameMode mode, int numPlayers, int playerSize, float moonRadius, Vector2 moonMiddle, float delta);
void DrawSnowball(Snowball* sb, Vector2 moonMiddle, float moonRadius);