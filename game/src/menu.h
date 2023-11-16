#pragma once
#include "input.h"
#include <raylib.h>

typedef struct MenuConfig {
	int numPlayers;
	PlayerInputConfig playerConfig[8];
}MenuConfig;

MenuConfig MenuLoop(Texture2D playerTex, Texture2D moonTex, Texture2D spaceTex);