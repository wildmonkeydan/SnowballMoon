#include <raylib.h>
#include "input.h"

#define ARROW_KEYS (KeyboardConfig) { KEY_LEFT, KEY_RIGHT, KEY_DOWN, KEY_UP}
#define WASD_KEYS (KeyboardConfig) { KEY_A, KEY_D, KEY_S, KEY_W}
#define IJKL_KEYS (KeyboardConfig) { KEY_J, KEY_L, KEY_K, KEY_I}
#define NUMPAD_KEYS (KeyboardConfig) { KEY_KP_4, KEY_KP_6, KEY_KP_5, KEY_KP_8}

bool input_GetButtonPressed(GameInput button, int id) {
	bool input_GetKeyConfigPressed(GameInput button, int config);

	Vector2 analog = { GetGamepadAxisMovement(id, GAMEPAD_AXIS_LEFT_X) , GetGamepadAxisMovement(id, GAMEPAD_AXIS_LEFT_Y) };

	switch (button) {
	case GI_LEFT:
		return IsGamepadButtonPressed(id, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || (analog.x < -0.5f) || input_GetKeyConfigPressed(button, id);
	case GI_RIGHT:
		return IsGamepadButtonPressed(id, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || (analog.x > 0.5f) || input_GetKeyConfigPressed(button, id);
	case GI_DOWN:
		return IsGamepadButtonPressed(id, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || (analog.y > 0.5f) || input_GetKeyConfigPressed(button, id);
	case GI_ATTACK:
		return IsGamepadButtonPressed(id, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || input_GetKeyConfigPressed(button, id);
	}
}

bool input_GetButtonUp(GameInput button, int id) {
	bool input_GetKeyConfigUp(GameInput button, int config);

	switch (button) {
	case GI_LEFT:
		return IsGamepadButtonReleased(id, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || input_GetKeyConfigUp(button, id);
	case GI_RIGHT:
		return IsGamepadButtonReleased(id, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || input_GetKeyConfigUp(button, id);
	case GI_DOWN:
		return IsGamepadButtonReleased(id, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || input_GetKeyConfigUp(button, id);
	case GI_UP:
		return IsGamepadButtonReleased(id, GAMEPAD_BUTTON_LEFT_FACE_UP) || input_GetKeyConfigUp(button, id);
	}
}

bool input_GetButton(GameInput button, int id) {
	bool input_GetKeyConfigDown(GameInput button, int config);

	Vector2 analog = { GetGamepadAxisMovement(id, GAMEPAD_AXIS_LEFT_X) , GetGamepadAxisMovement(id, GAMEPAD_AXIS_LEFT_Y) };

	switch (button) {
	case GI_LEFT:
		return IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || (analog.x < -0.5f) || input_GetKeyConfigDown(button, id);
	case GI_RIGHT:
		return IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || (analog.x > 0.5f) || input_GetKeyConfigDown(button, id);
	case GI_DOWN:
		return IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || (analog.y > 0.5f) || input_GetKeyConfigDown(button, id);
	case GI_UP:
		return IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_UP) || (analog.y < -0.5f) || input_GetKeyConfigDown(button, id);
	case GI_ATTACK:
		return IsGamepadButtonDown(id, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || input_GetKeyConfigDown(button, id);
	}
}


bool input_GetKeyConfigUp(GameInput button, int config) {
	KeyboardConfig keyConfig = { 0 };

	switch (config) {
	case 0:
		keyConfig = ARROW_KEYS;
		break;
	case 1:
		keyConfig = WASD_KEYS;
		break;
	case 2:
		keyConfig = IJKL_KEYS;
		break;
	case 3:
		keyConfig = NUMPAD_KEYS;
		break;
	}

	switch (button) {
	case GI_LEFT:
		return IsKeyUp(keyConfig.left);
	case GI_RIGHT:
		return IsKeyUp(keyConfig.right);
	case GI_DOWN:
		return IsKeyUp(keyConfig.down);
	case GI_ATTACK:
		return IsKeyUp(keyConfig.attack);
	}
}

bool input_GetKeyConfigPressed(GameInput button, int config) {
	KeyboardConfig keyConfig = { 0 };

	switch (config) {
	case 0:
		keyConfig = ARROW_KEYS;
		break;
	case 1:
		keyConfig = WASD_KEYS;
		break;
	case 2:
		keyConfig = IJKL_KEYS;
		break;
	case 3:
		keyConfig = NUMPAD_KEYS;
		break;
	}

	switch (button) {
	case GI_LEFT:
		return IsKeyPressed(keyConfig.left);
	case GI_RIGHT:
		return IsKeyPressed(keyConfig.right);
	case GI_DOWN:
		return IsKeyPressed(keyConfig.down);
	case GI_ATTACK:
		return IsKeyPressed(keyConfig.attack);
	}
}

bool input_GetKeyConfigDown(GameInput button, int config) {
	KeyboardConfig keyConfig = { 0 };

	switch (config) {
	case 0:
		keyConfig = ARROW_KEYS;
		break;
	case 1:
		keyConfig = WASD_KEYS;
		break;
	case 2:
		keyConfig = IJKL_KEYS;
		break;
	case 3:
		keyConfig = NUMPAD_KEYS;
		break;
	}

	switch (button) {
	case GI_LEFT:
		return IsKeyDown(keyConfig.left);
	case GI_RIGHT:
		return IsKeyDown(keyConfig.right);
	case GI_DOWN:
		return IsKeyDown(keyConfig.down);
	case GI_ATTACK:
		return IsKeyDown(keyConfig.attack);
	}
}


bool input_DetectInputConfig(PlayerInputConfig* config) {
	// Detect Gamepads
	for (int i = 0; i < 8; i++) {
		if (IsGamepadButtonPressed(i, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
			config = i + 4;
			return true;
		}
	}

	// Detect Keyboard Conifgs
	KeyboardKey key = GetKeyPressed();

	KeyboardConfig kbConfig = ARROW_KEYS;

	if (key == kbConfig.left || key == kbConfig.right || key == kbConfig.down || key == kbConfig.attack) {
		config = KEY_ARROW;
		return true;
	}

	kbConfig = WASD_KEYS;

	if (key == kbConfig.left || key == kbConfig.right || key == kbConfig.down || key == kbConfig.attack) {
		config = KEY_WASD;
		return true;
	}

	kbConfig = IJKL_KEYS;

	if (key == kbConfig.left || key == kbConfig.right || key == kbConfig.down || key == kbConfig.attack) {
		config = KEY_IJKL;
		return true;
	}

	kbConfig = NUMPAD_KEYS;

	if (key == kbConfig.left || key == kbConfig.right || key == kbConfig.down || key == kbConfig.attack) {
		config = KEY_NUMPAD;
		return true;
	}

	return false;
}