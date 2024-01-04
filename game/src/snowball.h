#pragma once
#include <raylib.h>
#include "types.h"

/// <summary>
/// Create a Snowball with the physics type Straight
/// </summary>
/// <param name="sb:">
/// Snowball to initialize
/// </param>
/// <param name="playerHeight:">
/// Player size in screen space
/// </param>
/// <param name="owner:">
/// Who threw this snowball?
/// </param>
/// <param name="angle:">
/// Angle to start from
/// </param>
/// <param name="direction:">
/// Direction to throw in
/// </param>
void CreateSnowballStraight(Snowball* sb, int playerHeight, int owner, float angle, int direction);

/// <summary>
/// Create a Snowball with the physics type Gravity
/// </summary>
/// <param name="sb:">
/// Snowball to initialize
/// </param>
/// <param name="owner:">
/// Time taken for the previous frame
/// </param>
/// <param name="angle:">
/// Angle to start from
/// </param>
/// <param name="moonMiddle:">
/// Center of the moon in screen space
/// </param>
/// <param name="moonRadius:">
/// Radius of the moon in screen space
/// </param>
/// <param name="direction:">
/// Direction to throw in
/// </param>
/// <param name="playerHeight:">
/// Player size in screen space
/// </param>
void CreateSnowballGravity(Snowball* sb, int owner, float angle, Vector2 moonMiddle, float moonRadius, Vector2 direction, int playerHeight);

/// <summary>
/// Update a Snowball, should be called every frame
/// </summary>
/// <param name="sb:">
/// Snowball to update
/// </param>
/// <param name="players:">
/// Pointer to array of players, used for collisions
/// </param>
/// <param name="forts:">
/// Pointer to array of forts, used for collisions
/// </param>
/// <param name="particles:">
/// Pointer to array of particles
/// </param>
/// <param name="nextParticle:">
/// Next particle to create in the array
/// </param>
/// <param name="mode:">
/// Game Mode
/// </param>
/// <param name="numPlayers:">
/// Number of players
/// </param>
/// <param name="playerSize:">
/// Player size in screen space
/// </param>
/// <param name="moonRadius:">
/// Radius of the moon in screen space
/// </param>
/// <param name="moonMiddle">
/// Center of the moon in screen space 
/// </param>
/// <param name="delta">
/// Time taken for the previous frame
/// </param>
/// <returns>
/// nextParticle
/// </returns>
int UpdateSnowball(Snowball* sb, Player* players, Fort* forts, Particle* particles, int nextParticle, GameMode mode, int numPlayers, int playerSize, float moonRadius, Vector2 moonMiddle, float delta);

/// <summary>
/// Draw a Snowball, should be called every frame after UpdateSnowball
/// </summary>
/// <param name="sb:">
/// Snowball to draw 
/// </param>
/// <param name="moonMiddle:">
/// Center of the moon in screen space
/// </param>
/// <param name="moonRadius:">
/// Radius of the moon in screen space
/// </param>
void DrawSnowball(Snowball* sb, Vector2 moonMiddle, float moonRadius);