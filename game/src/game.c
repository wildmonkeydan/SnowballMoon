#include <raylib.h>

#include "player.h"
#include "snowball.h"
#include "menu.h"
#include "fort.h"
#include "error.h"
#include "particle.h"

#define MAX_SNOWBALLS 16
#define MAX_PARTICLES 64
#define NUM_SOUNDS 10

void GameLoop() {
	bool GameModeUpdate(GameMode mode, Player* players, Fort* forts, int params[2], float timer, int numPlayers, int* winner);
	const char* GetPathFromMusic(MusicTrack track);
	int LoadSoundEffects(Sound* sounds);

	// =========
	// Init vars
	// =========

	float timer = 0.f;
	bool gameEnd = false;
	int gameWinner = -1;

	// Texture vars
	Texture2D playerTex = LoadTexture("Data/PlayerSprites.png");
	SetTextureFilter(playerTex, TEXTURE_FILTER_TRILINEAR);

	if (playerTex.id == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load PlayerSprites.png");
		return;
	}

	Texture2D moonTex = LoadTexture("Data/Moon.png");
	SetTextureFilter(moonTex, TEXTURE_FILTER_TRILINEAR);

	if (moonTex.id == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Moon.png");
		UnloadTexture(playerTex);
		return;
	}

	Texture2D spaceTex = LoadTexture("Data/Space.png");
	SetTextureFilter(spaceTex, TEXTURE_FILTER_TRILINEAR);
	SetTextureWrap(spaceTex, TEXTURE_WRAP_REPEAT);

	if (spaceTex.id == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Space.png");
		UnloadTexture(playerTex);
		UnloadTexture(moonTex);
		return;
	}

	Texture2D arrowTex = LoadTexture("Data/Arrow.png");
	SetTextureFilter(spaceTex, TEXTURE_FILTER_TRILINEAR);

	if (arrowTex.id == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Arrow.png");
		UnloadTexture(playerTex);
		UnloadTexture(moonTex);
		UnloadTexture(spaceTex);
		return;
	}

	Sound soundEffects[NUM_SOUNDS] = { 0 };

	if (LoadSoundEffects(soundEffects) == -1) {
		UnloadTexture(playerTex);
		UnloadTexture(moonTex);
		UnloadTexture(spaceTex);
		UnloadTexture(arrowTex);
		return;
	}

	MenuConfig config = MenuLoop(playerTex, spaceTex, soundEffects);

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
	bool musicOn = true;

	// Snowball vars
	Snowball snowballs[MAX_SNOWBALLS];
	int nextSnowball = 0;

	for (int i = 0; i < MAX_SNOWBALLS; i++) {
		snowballs[i].active = false;
	}

	Particle particles[MAX_PARTICLES];
	int nextParticle = 0;

	for (int i = 0; i < MAX_PARTICLES; i++) {
		particles[i].active = false;
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

		if (IsKeyPressed(KEY_F1)) { // Toggle music
			musicOn = !musicOn;
		}

		if (!gameEnd) {
			// Update Players
			for (int i = 0; i < config.numPlayers; i++) {
				nextSnowball = UpdatePlayer(&players[i], playerSize, delta, snowballs, nextSnowball, moonCenter, moonRadius);
			}

			// Update Snowballs
			for (int i = 0; i < MAX_PARTICLES; i++) {
				UpdateParticle(&particles[i], delta);
			}

			for (int i = 0; i < MAX_SNOWBALLS; i++) {
				nextParticle = UpdateSnowball(&snowballs[i], players, forts, particles, nextParticle, config.mode, 4, playerSize, moonRadius - moonRadiusAdjust, moonCenter, delta);
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

			if (musicOn) {
				UpdateMusicStream(track);
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
			DrawTexturePro(moonTex, 
				(Rectangle) { 0, 0, moonTex.width, moonTex.height }, 
				(Rectangle) { moonCenter.x - moonRadius, moonCenter.y - moonRadius, moonRadius * 2, moonRadius * 2 }, 
				(Vector2) { 0, 0 }, 0.f, RAYWHITE);

			// Draw Players
			for (int i = 0; i < config.numPlayers; i++) {
				DrawPlayer(&players[i], moonCenter, moonRadius - moonRadiusAdjust, playerTex, arrowTex, playerSize, delta, config.mode, soundEffects);
			}

			
			// Draw Snowballs
			for (int i = 0; i < MAX_PARTICLES; i++) {
				DrawParticle(&particles[i], moonRadius - moonRadiusAdjust);
			}

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

				StopMusicStream(track);
				UnloadMusicStream(track);

				config = MenuLoop(playerTex, spaceTex, soundEffects);

				

				if (config.numPlayers == -1) {					
					UnloadTexture(playerTex);
					UnloadTexture(moonTex);
					UnloadTexture(spaceTex);
					UnloadTexture(arrowTex);

					for (int i = 0; i < NUM_SOUNDS; i++) {
						UnloadSound(soundEffects[i]);
					}
					return;
				}

				track = LoadMusicStream(GetPathFromMusic(config.chosenTrack));
				PlayMusicStream(track);

				for (int i = 0; i < config.numPlayers; i++) {
					CreatePlayer(&players[i], playerSize, config.playerConfig[i], i, config.playerTeams[i]);
					players[i].angle = i * 30;
					players[i].colour = config.playerColours[i];
				}

				for (int i = 0; i < MAX_SNOWBALLS; i++) {
					snowballs[i].active = false;
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
				DrawPlayer(&players[i], moonCenter, moonRadius - moonRadiusAdjust, playerTex, arrowTex, playerSize, delta, config.mode, soundEffects);
			}

			// Draw Snowballs
			for (int i = 0; i < MAX_PARTICLES; i++){
				DrawParticle(&particles[i], moonRadius - moonRadiusAdjust);
			}

			for (int i = 0; i < MAX_SNOWBALLS; i++) {
				DrawSnowball(&snowballs[i], moonCenter, moonRadius - moonRadiusAdjust);
			}


			// Draw Scores
			for (int i = 0; i < config.numPlayers; i++) {
				DrawText(TextFormat("%d", players[i].score), (GetScreenWidth() / 9) * i + (((GetScreenWidth() / 9) / 8) * i), GetScreenHeight() - fontSize, fontSize, players[i].colour);
			}

			// Draw Result
			if (config.mode == GM_TEAM_FORT) {
				DrawTextPro(GetFontDefault(), 
					TextFormat("Team %d Wins!", gameWinner + 1), 
					(Vector2) { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f }, 
					(Vector2) { ((TextLength(TextFormat("Team %d Wins!", gameWinner + 1)) - 2) * (fontSize * 4)) / 4, fontSize },
					0.f, fontSize * 4, fontSize / 10, forts[gameWinner].teamColour);
			}
			else {
				DrawTextPro(GetFontDefault(), 
					TextFormat("Player %d Wins!", gameWinner + 1), 
					(Vector2) { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f }, 
					(Vector2) { ((TextLength(TextFormat("Player %d Wins!", gameWinner + 1)) - 2) * (fontSize * 4)) / 4, fontSize }, 
					0.f, fontSize * 4, fontSize / 10, players[gameWinner].colour);
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

	for (int i = 0; i < NUM_SOUNDS; i++) {
		UnloadSound(soundEffects[i]);
	}
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

	return gameEnd;
}

/// <summary>
/// Get the file path to load a MusicTrack from
/// </summary>
/// <param name="track:">
/// Track to get file path of
/// </param>
/// <returns>
/// String of filepath
/// </returns>
const char* GetPathFromMusic(MusicTrack track) {
	switch (track) {
	case MUS_LOBBY:
		return "Music/Lobby.mp3";
	case MUS_DIAMOND:
		return "Music/Black Diamond.mp3";
	case MUS_ARTIC:
		return "Music/Artic Beat.mp3";
	case MUS_ESKIMO:
		return "Music/Eskimo Zone.mp3";
	}
}

/// <summary>
/// Load in the sound effects array
/// </summary>
/// <param name="sounds:">
/// Array to load sounds into
/// </param>
/// <returns>
/// 0 is successful, -1 if an error occured
/// </returns>
int LoadSoundEffects(Sound* sounds) {

	sounds[SND_CONFIRM] = LoadSound("Data/Confirm.wav");

	if (sounds[SND_CONFIRM].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Confirm.wav");
		return -1;
	}

	sounds[SND_SCROLL] = LoadSound("Data/Scroll.wav");

	if (sounds[SND_SCROLL].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Scroll.wav");
		return -1;
	}

	sounds[SND_STEP0] = LoadSound("Data/Step0.wav");

	if (sounds[SND_STEP0].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Step0.wav");
		return -1;
	}

	sounds[SND_STEP1] = LoadSound("Data/Step1.wav");

	if (sounds[SND_STEP1].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Step1.wav");
		return -1;
	}

	sounds[SND_STEP2] = LoadSound("Data/Step2.wav");

	if (sounds[SND_STEP2].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Step2.wav");
		return -1;
	}

	sounds[SND_STEP3] = LoadSound("Data/Step3.wav");

	if (sounds[SND_STEP3].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Step3.wav");
		return -1;
	}

	sounds[SND_HIT] = LoadSound("Data/Hit.wav");

	if (sounds[SND_HIT].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Hit.wav");
		return -1;
	}

	sounds[SND_THROW] = LoadSound("Data/Throw.wav");

	if (sounds[SND_THROW].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Throw.wav");
		return -1;
	}

	sounds[SND_THUD] = LoadSound("Data/Thud.wav");

	if (sounds[SND_THUD].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Thud.wav");
		return -1;
	}

	sounds[SND_FORM] = LoadSound("Data/Form.wav");

	if (sounds[SND_FORM].stream.buffer == NULL) {
		ErrorTrap(ERROR_LOADING, "Could not load Form.wav");
		return -1;
	}

	return 0;
}