#include "menu.h"

MenuConfig MenuLoop(Texture2D playerTex, Texture2D moonTex, Texture2D spaceTex) {
	MenuConfig config = { 0 };

	config.numPlayers = 1;

	while (!WindowShouldClose()) {

		if (IsKeyPressed(KEY_ENTER))
			break;

		if (input_DetectInputConfig(config.playerConfig[config.numPlayers - 1])) {
			config.numPlayers++;
		}

		BeginDrawing();

		ClearBackground(BLACK);

		// Draw Space Background
		DrawTextureRec(spaceTex, (Rectangle) { 0, 0, GetScreenWidth(), GetScreenHeight() }, (Vector2) { 0, 0 }, RAYWHITE);

		EndDrawing();
	}

	return config;
}