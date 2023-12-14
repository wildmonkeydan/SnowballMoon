#pragma once
#include <raylib.h>

/// <summary>
/// Inputs for the game
/// </summary>
typedef enum GameInput{
	GI_LEFT,
	GI_RIGHT,
	GI_UP,
	GI_DOWN,
	GI_ATTACK,
	GI_PAUSE
}GameInput;

/// <summary>
/// Player Input Configurations
/// </summary>
typedef enum PlayerInputConfig {
	KEY_ARROW,
	KEY_WASD,
	KEY_IJKL,
	KEY_NUMPAD,
	GAMEPAD_0,
	GAMEPAD_1,
	GAMEPAD_2,
	GAMEPAD_3,
	GAMEPAD_4,
	GAMEPAD_5,
	GAMEPAD_6,
	GAMEPAD_7
}PlayerInputConfig;

/// <summary>
/// Configuration for keyboard players
/// </summary>
typedef struct KeyboardConifg{
	KeyboardKey left;
	KeyboardKey right;
	KeyboardKey down;
	KeyboardKey attack;
}KeyboardConfig;

/// <summary>
/// Get if the given input was pressed this frame
/// </summary>
/// <param name="button: Input to check for"></param>
/// <param name="id: Player id to check the input from"></param>
/// <returns> 
/// True if pressed this frame
/// </returns>
bool input_GetButtonPressed(GameInput button, int id);

/// <summary>
/// Get if the given input was released this frame
/// </summary>
/// <param name="button: Input to check for"></param>
/// <param name="id: Player id to check the input from"></param>
/// <returns>
/// True if released this frame
/// </returns>
bool input_GetButtonUp(GameInput button, int id);

/// <summary>
/// Get given input's status
/// </summary>
/// <param name="button: Input to check for"></param>
/// <param name="id: Player id to check the input from"></param>
/// <returns>
/// True if down
/// </returns>
bool input_GetButton(GameInput button, int id);

/// <summary>
/// Get if the current PlayerInputConfig is being pressed 
/// </summary>
/// <param name="config:">
/// The PlayerInputConfig to check inputs from
/// </param>
/// <returns>
/// If the PlayerInputConfig has been pressed
/// </returns>
bool input_DetectInputConfig(PlayerInputConfig* config);