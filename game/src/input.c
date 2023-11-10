#include <raylib.h>
#include "input.h"

bool input_GetButtonPressed(GameInput button, int id) {
	Vector2 analog = { GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) , GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) };

	switch (button) {
	case GI_LEFT:
		return IsGamepadButtonPressed(id, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || (analog.x < -0.5f) || IsKeyPressed(KEY_LEFT);
	case GI_RIGHT:
		return IsGamepadButtonPressed(id, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || (analog.x > 0.5f) || IsKeyPressed(KEY_RIGHT);
	case GI_DOWN:
		return IsGamepadButtonPressed(id, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || (analog.y > 0.5f) || IsKeyPressed(KEY_DOWN);
	case GI_UP:
		return IsGamepadButtonPressed(id, GAMEPAD_BUTTON_LEFT_FACE_UP) || (analog.y < -0.5f) || IsKeyPressed(KEY_UP);
	case GI_ATTACK:
		return IsGamepadButtonPressed(id, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || IsKeyPressed(KEY_Z);
	}
}

bool input_GetButtonUp(GameInput button, int id) {

	//TraceLog(LOG_INFO, "X: %d  Y: %d", analogXInUse, analogYInUse);

	switch (button) {
	case GI_LEFT:
		return IsGamepadButtonReleased(id, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || IsKeyReleased(KEY_LEFT);
	case GI_RIGHT:
		return IsGamepadButtonReleased(id, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || IsKeyReleased(KEY_RIGHT);
	case GI_DOWN:
		return IsGamepadButtonReleased(id, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || IsKeyReleased(KEY_DOWN);
	case GI_UP:
		return IsGamepadButtonReleased(id, GAMEPAD_BUTTON_LEFT_FACE_UP) || IsKeyReleased(KEY_UP);
	}
}

bool input_GetButton(GameInput button, int id) {
	Vector2 analog = { GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) , GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) };

	switch (button) {
	case GI_LEFT:
		return IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || (analog.x < -0.5f) || IsKeyDown(KEY_LEFT);
	case GI_RIGHT:
		return IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || (analog.x > 0.5f) || IsKeyDown(KEY_RIGHT);
	case GI_DOWN:
		return IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || (analog.y > 0.5f) || IsKeyDown(KEY_DOWN);
	case GI_UP:
		return IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_UP) || (analog.y < -0.5f) || IsKeyDown(KEY_UP);
	}
}