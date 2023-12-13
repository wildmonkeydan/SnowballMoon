#pragma once
#include <raylib.h>
#include "types.h"

void CreateFort(Fort* fort, int health, Color team, int id, Vector2 pos, int size, bool flip);
void DrawFort(Fort* fort, Texture2D fortTex, float delta, int fontSize);