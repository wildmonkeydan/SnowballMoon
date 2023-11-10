#pragma once

typedef enum {
	GI_LEFT,
	GI_RIGHT,
	GI_UP,
	GI_DOWN,
	GI_ATTACK,
	GI_PAUSE
}GameInput;

bool input_GetButtonPressed(GameInput button, int id);
bool input_GetButtonUp(GameInput button, int id);
bool input_GetButton(GameInput button, int id);