#pragma once
#include "input.h"
#include <raylib.h>

typedef struct MenuConfig {
	int numPlayers;
	PlayerInputConfig playerConfig[8];
	Color playerColours[8];
}MenuConfig;

typedef struct PlayerBlock {
	bool active;
	bool ready;
	Color chosenColour;
	short colourIndex;
	PlayerInputConfig playerConfig;
	int playstationControllerId;
}PlayerBlock;

MenuConfig MenuLoop(Texture2D playerTex, Texture2D moonTex, Texture2D spaceTex);