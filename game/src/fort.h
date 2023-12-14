#pragma once
#include <raylib.h>
#include "types.h"

/// <summary>
/// Create a fort with initial parameters
/// </summary>
/// <param name="fort:">
/// Fort to initialize
/// </param>
/// <param name="health:">
/// Max health
/// </param>
/// <param name="team:">
/// Colour of associated team
/// </param>
/// <param name="id:">
/// ID of current team
/// </param>
/// <param name="pos:">
/// Position in screen space
/// </param>
/// <param name="size:">
/// Size in screen space
/// </param>
/// <param name="flip:">
/// Is it at the bottom of the moon?
/// </param>
void CreateFort(Fort* fort, int health, Color team, int id, Vector2 pos, int size, bool flip);

/// <summary>
/// Draw a fort
/// </summary>
/// <param name="fort:">
/// Fort to draw
/// </param>
/// <param name="fortTex:">
/// Sprite atlas to use for drawing
/// </param>
/// <param name="delta:">
/// Time taken for the previous frame
/// </param>
/// <param name="fontSize:">
/// Font size in screen space
/// </param>
void DrawFort(Fort* fort, Texture2D fortTex, float delta, int fontSize);