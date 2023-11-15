#pragma once
#include <raylib.h>

/// <summary>
/// Inputs for the game
/// </summary>
typedef enum {
	GI_LEFT,
	GI_RIGHT,
	GI_UP,
	GI_DOWN,
	GI_ATTACK,
	GI_PAUSE
}GameInput;

/// <summary>
/// Configuration for keyboard players
/// </summary>
typedef struct {
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