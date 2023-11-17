#include "menu.h"
#include "types.h"

MenuConfig MenuLoop(Texture2D playerTex, Texture2D moonTex, Texture2D spaceTex) {
	Color GetPlayerColourFromIndex(int index);

	MenuConfig config = { 0 };
	config.numPlayers = 1;

	PlayerBlock blocks[8];
	for (int i = 0; i < 8; i++) {
		blocks[i].active = false;
		blocks[i].chosenColour = GetPlayerColourFromIndex(GetRandomValue(0, 7));
		blocks[i].ready = false;
		blocks[i].playerConfig = GAMEPAD_0;
	}
	blocks[0].active = true;

	int playerBlockSize = GetScreenWidth() / 9;
	Vector2 playerBlockOffset = { 0 , GetScreenHeight() - playerBlockSize };
	int fontSize = GetScreenHeight() / 40.f;

	while (!WindowShouldClose()) {

		if (IsKeyPressed(KEY_ENTER))
			break;

		if (input_DetectInputConfig(&config.playerConfig[config.numPlayers - 1])) {
			blocks[config.numPlayers - 1].active = true;
			blocks[config.numPlayers - 1].playerConfig = config.playerConfig[config.numPlayers - 1];
			if (config.numPlayers < 8) {
				config.numPlayers++;
			}
		}

		BeginDrawing();

		ClearBackground(BLACK);

		// Draw Space Background
		DrawTextureRec(spaceTex, (Rectangle) { 0, 0, GetScreenWidth(), GetScreenHeight() }, (Vector2) { 0, 0 }, RAYWHITE);

		// Draw Player Selection
		for (int i = 0; i < config.numPlayers; i++) {
			if (blocks[i].active) {
				DrawRectangleRounded((Rectangle) { playerBlockOffset.x + (i * playerBlockSize), playerBlockOffset.y, playerBlockSize, playerBlockSize }, playerBlockSize / 100.f, 3, blocks[i].ready ? GREEN : GRAY);
				char controlType[16] = "Controller ";

				if (blocks[i].playerConfig < 4) {
					switch (blocks[i].playerConfig) {
					case KEY_ARROW:
						TextCopy(controlType, "Arrow Keys");
						break;
					case KEY_WASD:
						TextCopy(controlType, "WASD Keys");
						break;
					case KEY_IJKL:
						TextCopy(controlType, "IJKL Keys");
						break;
					case KEY_NUMPAD:
						TextCopy(controlType, "Numpad Keys");
						break;
					}
				}
				else {
					int typePos = 12;
					TextAppend(controlType, TextFormat("%d", blocks[i].playerConfig), &typePos);
				}

				DrawText(controlType, playerBlockOffset.x + (i * playerBlockSize) + playerBlockSize / 10.f, playerBlockOffset.y + playerBlockSize / 100.f, fontSize, BLACK);
			}
		}

		EndDrawing();
	}

	return config;
}

Color GetPlayerColourFromIndex(int index) {
	switch (index) {
	case 0:
		return PLAYER_COLOUR_BLUE;
	case 1:
		return PLAYER_COLOUR_BROWN;
	case 2:
		return PLAYER_COLOUR_GREEN;
	case 3:
		return PLAYER_COLOUR_LIME;
	case 4:
		return PLAYER_COLOUR_NAVY;
	case 5:
		return PLAYER_COLOUR_ORANGE;
	case 6:
		return PLAYER_COLOUR_PINK;
	case 7:
		return PLAYER_COLOUR_VIOLET;
	}
}