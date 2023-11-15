#pragma once
#include <raylib.h>

typedef enum {
	GI_LEFT,
	GI_RIGHT,
	GI_UP,
	GI_DOWN,
	GI_ATTACK,
	GI_PAUSE
}GameInput;

typedef struct {
	KeyboardKey left;
	KeyboardKey right;
	KeyboardKey down;
	KeyboardKey attack;
}KeyboardConfig;

bool input_GetButtonPressed(GameInput button, int id);
bool input_GetButtonUp(GameInput button, int id);
bool input_GetButton(GameInput button, int id);