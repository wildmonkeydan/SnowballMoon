#include "menu.h"
#include "types.h"
#include "animation.h"
#include "JoyShock/JoyShockLibrary.h"
#include "error.h"

MenuConfig MenuLoop(Texture2D playerTex, Texture2D spaceTex) {
	Color GetPlayerColourFromIndex(int index);
	Color PlayerSelectColour(PlayerBlock* block);
	const char* GetTextFromGameMode(GameMode mode);
	void SelectGameMode(GameMode* mode);
	void UpdateGameModeParams(int params[2], GameMode mode);
	void DrawGameModeParams(int params[2], GameMode mode, int fontSize);
	MusicTrack UpdateMusicChoice(MusicMetadata* meta, MusicTrack track);
	void DrawMusicChoice(MusicMetadata* meta, MusicTrack track, int fontSize);
	void DrawInstructions(int fontSize);
	void DrawGameModeInstructions(int fontSize);

	MenuConfig config = { 0 };
	config.numPlayers = 1;
	config.mode = GM_FREE_FOR_ALL;
	config.modeParams[0] = 120;
	config.modeParams[1] = 10;

	PlayerBlock blocks[8];
	for (int i = 0; i < 8; i++) {
		short randColour = GetRandomValue(0, 7);

		blocks[i].active = false;
		blocks[i].chosenColour = GetPlayerColourFromIndex(randColour);
		blocks[i].ready = false;
		blocks[i].colourIndex = randColour;
		blocks[i].playerConfig = GAMEPAD_0;
		blocks[i].playstationControllerId = -1;
	}

	AnimContext idleCtx;
	animation_ClearContext(&idleCtx);
	idleCtx.loop = true;

	int playerBlockSize = GetScreenWidth() / 9;
	Vector2 playerBlockOffset = { 0 , GetScreenHeight() - playerBlockSize };
	int fontSize = GetScreenHeight() / 40.f;

	Texture2D logoTex = LoadTexture("Logo.png");
	SetTextureFilter(logoTex, TEXTURE_FILTER_TRILINEAR);

	Music lobbyMus = LoadMusicStream("Music/Lobby.mp3");
	PlayMusicStream(lobbyMus);

	MusicTrack musChoice = MUS_DIAMOND;
	MusicMetadata musMeta = { 0 };

	if (logoTex.id == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Logo.png");
		config.numPlayers = -1;
		return config;
	}

	int numPlaystation = JslConnectDevices();
	TraceLog(LOG_INFO, "Num Playstation Controllers: %d", numPlaystation);

	bool closingWindow = false;

	while (!closingWindow) {
		closingWindow = WindowShouldClose();

		int numReady = 0;

		// Ready up
		for (int i = 0; i < config.numPlayers; i++) {
			if (blocks[i].ready)
				numReady++;
		}

		if (numReady == config.numPlayers - 1 && config.numPlayers >= 3) {
			for (int i = 0; i < config.numPlayers; i++) {
				config.playerColours[i] = blocks[i].chosenColour;
			}
			config.numPlayers -= 1;
			config.chosenTrack = musChoice;

			break;
		}
		
		for (int i = 0; i < config.numPlayers; i++) {
			if (blocks[i].active) {
				if (input_GetButtonPressed(GI_ATTACK, config.playerConfig[i])) {
					blocks[i].ready = !blocks[i].ready;
				}

				Color change = PlayerSelectColour(&blocks[i]);


				if (change.a != 0) {
					blocks[i].chosenColour = change;
					if (blocks[i].playstationControllerId != -1) {
						Color mix = ColorBrightness(change, -0.5f);
						mix = (Color){ 0, change.r, change.g, change.b };

						JslSetLightColour(i, ColorToInt(mix));
					}
				}
			}
		}


		// Search for new players
		if (input_DetectInputConfig(&config.playerConfig[config.numPlayers - 1])) {
			bool usedConfig = false;

			for (int i = 0; i < config.numPlayers; i++) {
				if (config.playerConfig[config.numPlayers - 1] == config.playerConfig[i] && !(config.numPlayers - 1 == i)) {
					usedConfig = true;
				}
			}

			if (!usedConfig) {
				blocks[config.numPlayers - 1].active = true;
				blocks[config.numPlayers - 1].playerConfig = config.playerConfig[config.numPlayers - 1];

				if (blocks[config.numPlayers - 1].playerConfig >= 4) {
					// Check if Playstation Controller
					for (int i = 0; i < numPlaystation; i++) {
						int buttons = JslGetButtons(i);
						if (buttons & JSMASK_S) {
							Color switchColour = blocks[config.numPlayers - 1].chosenColour;
							switchColour.r = blocks[config.numPlayers - 1].chosenColour.b;
							switchColour.b = blocks[config.numPlayers - 1].chosenColour.r;
							JslSetLightColour(i, ColorToInt(ColorContrast(switchColour, -100.f)));
							blocks[config.numPlayers - 1].playstationControllerId = i;
						}
					}
				}

				if (config.numPlayers < 8) {
					config.numPlayers++;
				}
			}
		}

		SelectGameMode(&config.mode);
		UpdateGameModeParams(config.modeParams, config.mode);

		if (config.mode == GM_TEAM_FORT) {
			int split = (config.numPlayers / 2);

			for (int i = 0; i < config.numPlayers; i++) {
				if (i < split) {
					config.playerTeams[i] = 0;
				}
				else
				{
					config.playerTeams[i] = 1;
				}
			}
		}

		// Animate the characters
		Vector2 uv = animation_AnimateDef(DA_PLAYERIDLE, &idleCtx, GetFrameTime());

		musChoice = UpdateMusicChoice(&musMeta, musChoice);
		UpdateMusicStream(lobbyMus);

		BeginDrawing();

		ClearBackground(BLACK);

		// Draw Space Background
		DrawTextureRec(spaceTex, (Rectangle) { 0, 0, GetScreenWidth(), GetScreenHeight() }, (Vector2) { 0, 0 }, RAYWHITE);

		// Draw Logo
		DrawTexturePro(logoTex, (Rectangle) { 0, 0, logoTex.width, logoTex.height }, 
			(Rectangle) { (GetScreenWidth() / 2) + (GetScreenHeight() * 0.12525f), GetScreenHeight() / 15, GetScreenHeight() * 0.513f, GetScreenHeight() * 0.288f }, 
			(Vector2) { 416, 0 }, 0.f, RAYWHITE);

		// Draw Round Info
		DrawTextPro(GetFontDefault(), GetTextFromGameMode(config.mode), 
			(Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 3 }, 
			(Vector2) { (TextLength(GetTextFromGameMode(config.mode)) * (fontSize * 2)) / 4, 0 }, 
			0.f, fontSize * 2, fontSize / 10, RAYWHITE);
		DrawGameModeParams(config.modeParams, config.mode, fontSize);
		DrawMusicChoice(&musMeta, musChoice, fontSize);

		if (config.numPlayers == 1) {
			DrawInstructions(fontSize);
		}
		DrawGameModeInstructions(fontSize);

		// Draw Player Selection
		for (int i = 0; i < config.numPlayers; i++) {
			if (blocks[i].active) {
				int blockX = playerBlockOffset.x + (i * playerBlockSize) + ((playerBlockSize / 8) * i);

				DrawTexturePro(playerTex, (Rectangle) { uv.x, uv.y, 128, 128 }, 
					(Rectangle) { blockX, playerBlockOffset.y - playerBlockSize, playerBlockSize, playerBlockSize }, 
					(Vector2) { 0, 0 }, 0.f, blocks[i].chosenColour);

				if (config.mode == GM_TEAM_FORT) {
					DrawRectangle(blockX, playerBlockOffset.y, playerBlockSize, playerBlockSize, config.playerTeams[i] ? BLUE : RED);
				}

				DrawRectangleRounded((Rectangle) { blockX, playerBlockOffset.y, playerBlockSize, playerBlockSize }, playerBlockSize / 800.f, 3, blocks[i].ready ? GREEN : GRAY);
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
					TextCopy(controlType, TextFormat("Controller %d", (int)blocks[i].playerConfig - 3));
				}

				DrawText(controlType, blockX + playerBlockSize / 10.f, playerBlockOffset.y + playerBlockSize / 3.f, fontSize, BLACK);
			}
		}

		EndDrawing();
	}

	UnloadTexture(logoTex);
	UnloadMusicStream(lobbyMus);

	if (closingWindow) {
		config.numPlayers = -1; // If ESC is pressed instead of all player's readying up, close game
	}

	return config;
}

/// <summary>
/// Get a colour from the player palette using an index
/// </summary>
/// <param name="index">
/// Position in the palette to take the colour from
/// </param>
/// <returns>
/// Colour from the palette
/// </returns>
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

/// <summary>
/// Select a colour using the player's inputs
/// </summary>
/// <param name="block:">
/// Player changing the colour
/// </param>
/// <returns>
/// Colour to change to
/// </returns>
Color PlayerSelectColour(PlayerBlock* block) {
	if (input_GetButtonPressed(GI_LEFT, block->playerConfig)) {
		block->colourIndex--;

		if (block->colourIndex < 0) {
			block->colourIndex = 7;
		}

		return GetPlayerColourFromIndex(block->colourIndex);
	}
	if (input_GetButtonPressed(GI_RIGHT, block->playerConfig)) {
		block->colourIndex++;

		if (block->colourIndex > 7) {
			block->colourIndex = 0;
		}

		return GetPlayerColourFromIndex(block->colourIndex);
	}

	return CLITERAL(Color) { 0, 0, 0, 0 };
}

/// <summary>
/// Get a string describing the current selected game mode
/// </summary>
/// <param name="mode:">
/// Current game mode
/// </param>
/// <returns>
/// String describing game mode
/// </returns>
const char* GetTextFromGameMode(GameMode mode) {
	switch (mode) {
	case GM_FREE_FOR_ALL:
		return "Free for all";
	case GM_HOARDER:
		return "Hoarder";
	case GM_TEAM_FORT:
		return "Team Fort";
	default:
		return "ERROR";
	}
}

/// <summary>
/// Select a Game Mode using the number keys
/// </summary>
/// <param name="mode:">
/// GameMode variable to change
/// </param>
void SelectGameMode(GameMode* mode) {
	if (IsKeyPressed(KEY_ONE))
		*mode = GM_FREE_FOR_ALL;

	if (IsKeyPressed(KEY_TWO))
		*mode = GM_HOARDER;

	if (IsKeyPressed(KEY_THREE))
		*mode = GM_TEAM_FORT;
}

/// <summary>
/// Update the Game Mode parameters with PAGE_UP, PAGE_DOWN & NUMPAD +, NUMPAD -
/// </summary>
/// <param name="params:">
///	Parameters to update
/// </param>
/// <param name="mode":>
/// Current Game Mode
/// </param>
void UpdateGameModeParams(int params[2], GameMode mode) {
	if (IsKeyPressed(KEY_KP_ADD)) {
		params[0] += 60;
	}
	if (IsKeyPressed(KEY_KP_SUBTRACT) && params[0] > 0) {
		params[0] -= 60;
	}

	if (IsKeyPressed(KEY_PAGE_UP)) {
		params[1] += 5;
	}
	if (IsKeyPressed(KEY_PAGE_DOWN) && params[1] > 0) {
		params[1] -= 5;
	}
}

/// <summary>
/// Draw the Game Mode parameters
/// </summary>
/// <param name="params:">
/// Game Mode parameters
/// </param>
/// <param name="mode:">
/// Game Mode
/// </param>
/// <param name="fontSize:">
/// Size in screen space for fonts
/// </param>
void DrawGameModeParams(int params[2], GameMode mode, int fontSize) {
	switch (mode) {
	case GM_FREE_FOR_ALL:
		DrawTextPro(GetFontDefault(), TextFormat("Timer: %d", params[0]), (Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 3 + (fontSize) + (fontSize * 2) }, (Vector2) { (TextLength(TextFormat("Timer: %d", params[0])) * (fontSize)) / 4, 0 }, 0.f, fontSize, fontSize / 10, RAYWHITE);
		DrawTextPro(GetFontDefault(), TextFormat("Score Limit: %d", params[1]), (Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 3 + (fontSize * 4) }, (Vector2) { (TextLength(TextFormat("Score Limit: %d", params[1])) * (fontSize)) / 4, 0 }, 0.f, fontSize, fontSize / 10, RAYWHITE);
		break;
	case GM_HOARDER:
		DrawTextPro(GetFontDefault(), TextFormat("Timer: %d", params[0]), (Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 3 + (fontSize) + (fontSize * 2) }, (Vector2) { (TextLength(TextFormat("Timer: %d", params[0])) * (fontSize)) / 4, 0 }, 0.f, fontSize, fontSize / 10, RAYWHITE);
		DrawTextPro(GetFontDefault(), TextFormat("Snowball Limit: %d", params[1]), (Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 3 + (fontSize * 4) }, (Vector2) { (TextLength(TextFormat("Snowball Limit: %d", params[1])) * (fontSize)) / 4, 0 }, 0.f, fontSize, fontSize / 10, RAYWHITE);
		break;
	case GM_TEAM_FORT:
		DrawTextPro(GetFontDefault(), TextFormat("Timer: %d", params[0]), (Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 3 + (fontSize)+(fontSize * 2) }, (Vector2) { (TextLength(TextFormat("Timer: %d", params[0])) * (fontSize)) / 4, 0 }, 0.f, fontSize, fontSize / 10, RAYWHITE);
		DrawTextPro(GetFontDefault(), TextFormat("Fort Health: %d", params[1]), (Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 3 + (fontSize * 4) }, (Vector2) { (TextLength(TextFormat("Fort Health: %d", params[1])) * (fontSize)) / 4, 0 }, 0.f, fontSize, fontSize / 10, RAYWHITE);
	default:
		break;
	}


}

/// <summary>
/// Update the Music choice
/// </summary>
/// <param name="meta:">
/// Metadata of the chosen music
/// </param>
/// <param name="track:">
/// Current chosen MusicTrack
/// </param>
/// <returns>
/// MusicTrack chosen
/// </returns>
MusicTrack UpdateMusicChoice(MusicMetadata* meta, MusicTrack track) {
	if (IsKeyPressed(KEY_INSERT)) {
		if (track == 3) {
			track = 0;
		}
		else {
			track++;
		}
	}

	if (IsKeyPressed(KEY_DELETE)) {
		if (track == 0) {
			track = 3;
		}
		else {
			track--;
		}
	}

	switch (track) {
	case MUS_LOBBY:
		TextCopy(meta->artist, "Matthew Pablo");
		TextCopy(meta->name, "Snowland Town");
		break;
	case MUS_DIAMOND:
		TextCopy(meta->artist, "Joth");
		TextCopy(meta->name, "Black Diamond");
		break;
	case MUS_ESKIMO:
		TextCopy(meta->artist, "Macro");
		TextCopy(meta->name, "Eskimo Zone");
		break;
	case MUS_ARTIC:
		TextCopy(meta->artist, "Ted Kerr");
		TextCopy(meta->name, "Artic Beat");
		break;
	}

	return track;
}

/// <summary>
/// Draw the metadata of the current chosen music
/// </summary>
/// <param name="meta:">
/// Metadata of the chosen music
/// </param>
/// <param name="track:">
/// Current chosen MusicTrack
/// </param>
/// <param name="fontSize:">
/// Size in screen space for fonts
/// </param>
void DrawMusicChoice(MusicMetadata* meta, MusicTrack track, int fontSize) {
	int margin = GetScreenWidth() - (fontSize * 17);

	DrawText(meta->artist, margin, fontSize, fontSize, RAYWHITE);
	DrawText(meta->name, margin, (fontSize * 2) + (fontSize / 2), fontSize, RAYWHITE);
	DrawText("Use Insert/Delete to choose music", margin - fontSize, (fontSize * 4), fontSize, RAYWHITE);
}

/// <summary>
/// Draw instructions on how to start the game
/// </summary>
/// <param name="fontSize:">
/// Size in screen space for fonts
/// </param>
void DrawInstructions(int fontSize) {
	DrawText("Press a button of the gamepad/keys you want to use to enter the game, then press the ACTION button to ready up.", fontSize, GetScreenHeight() - fontSize, fontSize, RAYWHITE);
}

/// <summary>
/// Draw instructions on how to change GameMode
/// </summary>
/// <param name="fontSize:">
/// Size in screen space for fonts
/// </param>
void DrawGameModeInstructions(int fontSize) {
	DrawText("Use PgUp/PgDown to change score limit, Numpad +/- to change the timer", fontSize, fontSize, fontSize, RAYWHITE);
	DrawText("Use the Number Keys to change the game mode", fontSize, (fontSize * 2) + (fontSize / 4), fontSize, RAYWHITE);
}