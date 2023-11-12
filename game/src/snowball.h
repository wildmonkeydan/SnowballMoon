#pragma once
#include <raylib.h>
#include "types.h"

void CreateSnowballStraight(Snowball* sb, int owner, float angle, int direction);
void CreateSnowballGravity(Snowball* sb, int owner, float angle, Vector2 moonMiddle, float moonRadius, Vector2 velocity);
void UpdateSnowball(Snowball* sb, Player* players, int numPlayers, float moonRadius, Vector2 moonMiddle, float delta);
void DrawSnowball(Snowball* sb, Vector2 moonMiddle, float moonRadius);