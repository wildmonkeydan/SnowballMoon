#include <raylib.h>

#include "player.h"
#include "snowball.h"
#include "menu.h"
#include "fort.h"
#include "error.h"

#define MAX_SNOWBALLS 16

void GameLoop() {
	bool GameModeUpdate(GameMode mode, Player * players, Fort * forts, int params[2], float timer, int numPlayers, int* winner);
	const char* GetPathFromMusic(MusicTrack track);

	// =========
	// Init vars
	// =========

	float timer = 0.f;
	bool gameEnd = false;
	int gameWinner = -1;

	// Texture vars
	Texture2D playerTex = LoadTexture("PlayerSprites.png");
	SetTextureFilter(playerTex, TEXTURE_FILTER_POINT);

	if (playerTex.id == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load PlayerSprites.png");
		return;
	}

	Texture2D moonTex = LoadTexture("Moon.png");
	SetTextureFilter(moonTex, TEXTURE_FILTER_POINT);

	if (moonTex.id == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Moon.png");
		UnloadTexture(playerTex);
		return;
	}

	Texture2D spaceTex = LoadTexture("Space.png");
	SetTextureFilter(spaceTex, TEXTURE_FILTER_TRILINEAR);
	SetTextureWrap(spaceTex, TEXTURE_WRAP_REPEAT);

	if (spaceTex.id == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Space.png");
		UnloadTexture(playerTex);
		UnloadTexture(moonTex);
		return;
	}

	Texture2D arrowTex = LoadTexture("Arrow.png");
	SetTextureFilter(spaceTex, TEXTURE_FILTER_POINT);

	if (arrowTex.id == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Arrow.png");
		UnloadTexture(playerTex);
		UnloadTexture(moonTex);
		UnloadTexture(spaceTex);
		return;
	}


	MenuConfig config = MenuLoop(playerTex, spaceTex);

	if (config.numPlayers == -1) {
		UnloadTexture(playerTex);
		UnloadTexture(moonTex);
		UnloadTexture(spaceTex);
		UnloadTexture(arrowTex);

		return;
	}

	// Music vars
	Music track = LoadMusicStream(GetPathFromMusic(config.chosenTrack));
	PlayMusicStream(track);

	// Snowball vars
	Snowball snowballs[MAX_SNOWBALLS];
	int nextSnowball = 0;

	for (int i = 0; i < MAX_SNOWBALLS; i++) {
		snowballs[i].active = false;
	}

	// Moon vars
	float moonRadius = GetScreenHeight() / 2.f - (GetScreenHeight() * 0.15f);
	Vector2 moonCenter = {  GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };
	int moonRadiusAdjust = GetScreenWidth() / 384;
	int playerSize = moonRadius / 5;
	int fontSize = GetScreenHeight() / 40.f;
	
	// Fort vars
	Fort forts[2];
	CreateFort(&forts[0], config.modeParams[1], RED, 0, (Vector2) { moonCenter.x - playerSize, (moonCenter.y - moonRadius) - playerSize * 1.75f }, playerSize * 2, false);
	CreateFort(&forts[1], config.modeParams[1], BLUE, 1, (Vector2) { moonCenter.x - playerSize, (moonCenter.y + moonRadius) - playerSize * 0.25f }, playerSize * 2, true);

	// Player vars
	Player players[8];
	
	for (int i = 0; i < config.numPlayers; i++) {
		CreatePlayer(&players[i], playerSize, config.playerConfig[i], i, config.playerTeams[i]);
		players[i].angle = i * 30;
		players[i].colour = config.playerColours[i];
	}

	//===========
	// Main Loop
	//===========

	while (!WindowShouldClose()) {
		float delta = GetFrameTime();
		timer += delta;

		if (!gameEnd) {
			// Update Players
			for (int i = 0; i < config.numPlayers; i++) {
				nextSnowball = UpdatePlayer(&players[i], playerSize, delta, snowballs, nextSnowball, moonCenter, moonRadius);
			}

			// Update Snowballs
			for (int i = 0; i < MAX_SNOWBALLS; i++) {
				UpdateSnowball(&snowballs[i], players, forts, config.mode, 4, playerSize, moonRadius - moonRadiusAdjust, moonCenter, delta);
			}

			int winner = -1;
			if (GameModeUpdate(config.mode, players, forts, config.modeParams, timer, config.numPlayers, &winner)) {
				gameEnd = true; gameWinner = winner;  timer = 0;
				if (winner == -1) {
					UnloadTexture(playerTex);
					UnloadTexture(moonTex);
					UnloadTexture(spaceTex);
					UnloadTexture(arrowTex);
					return;
				}
			}

			UpdateMusicStream(track);

			BeginDrawing();

			ClearBackground(BLACK);

			// Draw Space Background
			DrawTextureRec(spaceTex, (Rectangle) { 0, 0, GetScreenWidth(), GetScreenHeight() }, (Vector2) { 0, 0 }, RAYWHITE);
			
			// Draw Forts
			if (config.mode == GM_TEAM_FORT) {
				for (int i = 0; i < 2; i++) {
					DrawFort(&forts[i], playerTex, delta, fontSize);
				}
			}

			// Draw Moon 
			DrawTexturePro(moonTex, (Rectangle) { 0, 0, moonTex.width, moonTex.height }, (Rectangle) { moonCenter.x - moonRadius, moonCenter.y - moonRadius, moonRadius * 2, moonRadius * 2 }, (Vector2) { 0, 0 }, 0.f, RAYWHITE);

			// Draw Players
			for (int i = 0; i < config.numPlayers; i++) {
				DrawPlayer(&players[i], moonCenter, moonRadius - moonRadiusAdjust, playerTex, arrowTex, playerSize, delta, config.mode);
			}

			
			// Draw Snowballs
			for (int i = 0; i < MAX_SNOWBALLS; i++) {
				DrawSnowball(&snowballs[i], moonCenter, moonRadius - moonRadiusAdjust);
			}


			// Draw Scores
			for (int i = 0; i < config.numPlayers; i++) {
				DrawText(TextFormat("%d", players[i].score), (GetScreenWidth() / 9) * i + (((GetScreenWidth() / 9) / 8) * i), GetScreenHeight() - fontSize, fontSize, players[i].colour);
			}

			EndDrawing();
		}
		else {
			if (timer > 10) {
				gameEnd = false;
				timer = 0;

				for (int i = 0; i < config.numPlayers; i++) {
					DestroyPlayer(&players[i]);
				}

				config = MenuLoop(playerTex, spaceTex);

				for (int i = 0; i < config.numPlayers; i++) {
					CreatePlayer(&players[i], playerSize, config.playerConfig[i], i, config.playerTeams[i]);
					players[i].angle = i * 30;
					players[i].colour = config.playerColours[i];
				}

				for (int i = 0; i < MAX_SNOWBALLS; i++) {
					snowballs[i].active = false;
				}
			}

			BeginDrawing();

			ClearBackground(BLACK);

			// Draw Space Background
			DrawTextureRec(spaceTex, (Rectangle) { 0, 0, GetScreenWidth(), GetScreenHeight() }, (Vector2) { 0, 0 }, RAYWHITE);

			// Draw Forts
			if (config.mode == GM_TEAM_FORT) {
				for (int i = 0; i < 2; i++) {
					DrawFort(&forts[i], playerTex, delta, fontSize);
				}
			}

			// Draw Moon 
			DrawTexturePro(moonTex, (Rectangle) { 0, 0, moonTex.width, moonTex.height }, (Rectangle) { moonCenter.x - moonRadius, moonCenter.y - moonRadius, moonRadius * 2, moonRadius * 2 }, (Vector2) { 0, 0 }, 0.f, RAYWHITE);

			// Draw Players
			for (int i = 0; i < config.numPlayers; i++) {
				DrawPlayer(&players[i], moonCenter, moonRadius - moonRadiusAdjust, playerTex, arrowTex, playerSize, delta, config.mode);
			}

			// Draw Snowballs
			for (int i = 0; i < MAX_SNOWBALLS; i++) {
				DrawSnowball(&snowballs[i], moonCenter, moonRadius - moonRadiusAdjust);
			}


			// Draw Scores
			for (int i = 0; i < config.numPlayers; i++) {
				DrawText(TextFormat("%d", players[i].score), (GetScreenWidth() / 9) * i + (((GetScreenWidth() / 9) / 8) * i), GetScreenHeight() - fontSize, fontSize, players[i].colour);
			}

			// Draw Result
			if (config.mode == GM_TEAM_FORT) {
				DrawTextPro(GetFontDefault(), TextFormat("Team %d Wins!", gameWinner + 1), (Vector2) { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f }, (Vector2) { ((TextLength(TextFormat("Team %d Wins!", gameWinner + 1)) - 1) * (fontSize * 4)) / 4, fontSize }, 0.f, fontSize * 4, fontSize / 10, forts[gameWinner].teamColour);
			}
			else {
				DrawTextPro(GetFontDefault(), TextFormat("Player %d Wins!", gameWinner + 1), (Vector2) { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f }, (Vector2) { ((TextLength(TextFormat("Player %d Wins!", gameWinner + 1)) - 1) * (fontSize * 4)) / 4, fontSize }, 0.f, fontSize * 4, fontSize / 10, players[gameWinner].colour);
			}

			EndDrawing();
		}
	}

	//=================
	// Quit and Unload
	//=================

	StopMusicStream(track);
	UnloadTexture(playerTex);
	UnloadTexture(moonTex);
	UnloadTexture(spaceTex);
	UnloadTexture(arrowTex);
	UnloadMusicStream(track);
}

/// <summary>
/// Check if the game has ended as dictated by the Game Mode
/// </summary>
/// <param name="mode:">
/// Current Game Mode
/// </param>
/// <param name="players:">
/// Pointer to array of players
/// </param>
/// <param name="forts:">
/// Pointer to array of forts
/// </param>
/// <param name="params:">
/// Game Mode parameters
/// </param>
/// <param name="timer:">
/// Current game timer
/// </param>
/// <param name="numPlayers:">
/// Number of players
/// </param>
/// <param name="winner:">
/// Set this to winning player/team
/// </param>
/// <returns>
/// If the game is over
/// </returns>
bool GameModeUpdate(GameMode mode, Player* players, Fort* forts, int params[2], float timer, int numPlayers, int* winner) {
	bool gameEnd = false;
	int max = players[0].score;
	int maxId = 0;

	switch (mode) {
	case GM_FREE_FOR_ALL:
		if (timer >= params[0]) {
			for (int i = 0; i < numPlayers; i++) {
				if (players[i].score > max) {
					maxId = i;
					max = players[i].score;
				}
			}
			*winner = maxId;
			gameEnd = true;
		}

		for (int i = 0; i < numPlayers; i++) {
			if (players[i].score >= params[1]) {
				*winner = i;
				gameEnd = true;
			}
		}
		break;
	case GM_HOARDER:
		for (int i = 0; i < numPlayers; i++) {
			if (players[i].snowballCount >= params[1]) {
				*winner = i;
				gameEnd = true;
			}
		}
		break;
	case GM_TEAM_FORT:
		for (int i = 0; i < 2; i++) {
			if (forts[i].health <= 0) {
				*winner = !i;
				gameEnd = true;
			}
		}
		break;
	default:
		ErrorTrap(ERROR_GAMELOGIC, "Invalid GameMode! Corruption may have occured");
		gameEnd = true;
		*winner = -1;
		break;
	}

	return gameEnd;
}

const char* GetPathFromMusic(MusicTrack track) {
	switch (track) {
	case MUS_LOBBY:
		return "Music/Lobby.mp3";
	case MUS_DIAMOND:
		return "Music/Black Diamond.mp3";
	}
}