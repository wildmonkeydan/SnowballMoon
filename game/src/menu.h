#pragma once
#include "input.h"
#include <raylib.h>
#include "types.h"

/// <summary>
/// Configuration for the game mode
/// </summary>
typedef struct MenuConfig {
	int numPlayers;
	PlayerInputConfig playerConfig[8];
	Color playerColours[8];
	unsigned char playerTeams[8];
	GameMode mode;
	int modeParams[2];
	MusicTrack chosenTrack;
}MenuConfig;

/// <summary>
/// Data type for player selection
/// </summary>
typedef struct PlayerBlock {
	bool active;
	bool ready;
	Color chosenColour;
	short colourIndex;
	PlayerInputConfig playerConfig;
	int playstationControllerId;
}PlayerBlock;

/// <summary>
/// Data type for displaying info on music tracks
/// </summary>
typedef struct MusicMetadata {
	char name[16];
	char artist[16];
}MusicMetadata;

/// <summary>
/// Main loop for menu
/// </summary>
/// <param name="playerTex:">
/// The sprite atlas used for the player
/// </param>
/// <param name="spaceTex:">
/// Repeating texture for the space background
/// </param>
/// <param name="sounds:">
/// Array of sound effects
/// </param>
/// <returns>
/// Configuration for the game
/// </returns>
MenuConfig MenuLoop(Texture2D playerTex, Texture2D spaceTex, Sound* sounds);