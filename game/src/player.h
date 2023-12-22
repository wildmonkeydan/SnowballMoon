#pragma once
#include <raylib.h>
#include "animation.h"
#include "types.h"

/// <summary>
/// Create a player using initial parameters
/// </summary>
/// <param name="player:">
/// Player to initialize
/// </param>
/// <param name="playerSize:">
/// Size of player in screen space
/// </param>
/// <param name="id:">
/// ID, used for inputs
/// </param>
/// <param name="playerId:">
/// PlayerID, used for collision response
/// </param>
/// <param name="team:">
/// The ID of the player's team
/// </param>
void CreatePlayer(Player* player, int playerSize, int id, int playerId, unsigned char team);

/// <summary>
/// Update a player, should be called every frame
/// </summary>
/// <param name="player:">
/// Player to update
/// </param>
/// <param name="playerSize:">
/// Size of player in screen space
/// </param>
/// <param name="delta:">
/// Time taken for the previous frame
/// </param>
/// <param name="sb:">
/// Snowball array
/// </param>
/// <param name="nextSnowball:">
/// Index of the next snowball in sb
/// </param>
/// <param name="moonMiddle:">
/// The center of the moon in screen space
/// </param>
/// <param name="moonRadius:">
/// The radius of the moon in screen space
/// </param>
/// <returns>
/// Index of the next snowball in sb
/// </returns>
int UpdatePlayer(Player* player, int playerSize, float delta, Snowball* sb, int nextSnowball, Vector2 moonMiddle, float moonRadius);

/// <summary>
/// Draw a player, should be called every frame after UpdatePlayer
/// </summary>
/// <param name="player:">
/// Player to draw
/// </param>
/// <param name="moonMiddle:">
/// The center of the moon in screen space
/// </param>
/// <param name="moonRadius:">
/// The radius of the moon in screen space
/// </param>
/// <param name="playerTex:">
/// The sprite atlas used for the player
/// </param>
/// <param name="arrowTex:">
/// The arrow sprite used in aiming
/// </param>
/// <param name="playerSize:">
/// Size of player in screen space
/// </param>
/// <param name="delta:">
/// Time taken for the previous frame
/// </param>
/// <param name="mode:">
/// Current game mode
/// </param>
void DrawPlayer(Player* player, Vector2 moonMiddle, float moonRadius, Texture2D playerTex, Texture2D arrowTex, int playerSize, float delta, GameMode mode);

/// <summary>
/// Destroy and invalidate a player
/// </summary>
/// <param name="player:">
/// Player to destroy
/// </param>
void DestroyPlayer(Player* player);