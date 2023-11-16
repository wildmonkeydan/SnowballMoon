#include <raylib.h>
#include "player.h"
#include "snowball.h"
#include "menu.h"

#define MAX_SNOWBALLS 16

void GameLoop() {
	// Init vars


	// Texture vars
	Texture2D playerTex = LoadTexture("PlayerSprites.png");
	SetTextureFilter(playerTex, TEXTURE_FILTER_TRILINEAR);

	Texture2D moonTex = LoadTexture("Moon.png");
	SetTextureFilter(moonTex, TEXTURE_FILTER_TRILINEAR);

	Texture2D spaceTex = LoadTexture("Space.png");
	SetTextureFilter(spaceTex, TEXTURE_FILTER_TRILINEAR);
	SetTextureWrap(spaceTex, TEXTURE_WRAP_REPEAT);

	MenuConfig config = MenuLoop(playerTex, moonTex, spaceTex);

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

	// Player vars
	Player players[4];
  
	CreatePlayer(&players[0], playerSize, 0);
	CreatePlayer(&players[1], playerSize, 1);
	CreatePlayer(&players[2], playerSize, 2);
	CreatePlayer(&players[3], playerSize, 3);

	players[1].angle = 30;
	players[1].colour = GREEN;

	players[2].angle = 60;
	players[2].colour = BLUE;

	players[3].angle = 90;
	players[3].colour = PURPLE;

	while (!WindowShouldClose()) {
		float delta = GetFrameTime();

		// Update Players
		nextSnowball = UpdatePlayer(&players[0], playerSize, delta, snowballs, nextSnowball);
		nextSnowball = UpdatePlayer(&players[1], playerSize, delta, snowballs, nextSnowball);
		nextSnowball = UpdatePlayer(&players[2], playerSize, delta, snowballs, nextSnowball);
		nextSnowball = UpdatePlayer(&players[3], playerSize, delta, snowballs, nextSnowball);

		// Update Snowballs
		for (int i = 0; i < MAX_SNOWBALLS; i++) {
			UpdateSnowball(&snowballs[i], players, 4, playerSize, moonRadius - moonRadiusAdjust, moonCenter, delta);
		}

		BeginDrawing();

		ClearBackground(BLACK);

		// Draw Space Background
		DrawTextureRec(spaceTex, (Rectangle) { 0, 0, GetScreenWidth(), GetScreenHeight() }, (Vector2) { 0, 0 }, RAYWHITE);

		// Draw Moon 
		DrawTexturePro(moonTex, (Rectangle) { 0, 0, moonTex.width, moonTex.height }, (Rectangle) { moonCenter.x - moonRadius, moonCenter.y - moonRadius, moonRadius * 2, moonRadius * 2 }, (Vector2) { 0, 0 }, 0.f, RAYWHITE);
		
		// Draw Players
		DrawPlayer(&players[0], moonCenter, moonRadius - moonRadiusAdjust, playerTex, playerSize, delta);
		DrawPlayer(&players[1], moonCenter, moonRadius - moonRadiusAdjust, playerTex, playerSize, delta);
		DrawPlayer(&players[2], moonCenter, moonRadius - moonRadiusAdjust, playerTex, playerSize, delta);
		DrawPlayer(&players[3], moonCenter, moonRadius - moonRadiusAdjust, playerTex, playerSize, delta);

		// Draw Snowballs
		for (int i = 0; i < MAX_SNOWBALLS; i++) {
			DrawSnowball(&snowballs[i], moonCenter, moonRadius - moonRadiusAdjust);
		}


		// Draw Scores
		DrawText(TextFormat("%d", players[0].score), 0, 0, fontSize, players[0].colour);
		DrawText(TextFormat("%d", players[1].score), GetScreenWidth() - (fontSize * 3), 0, fontSize, players[1].colour);
		DrawText(TextFormat("%d", players[2].score), 0, GetScreenHeight() - fontSize, fontSize, players[2].colour);
		DrawText(TextFormat("%d", players[3].score), GetScreenWidth() - (fontSize * 3), GetScreenHeight() - fontSize, fontSize, players[3].colour);

		EndDrawing();
	}

	UnloadTexture(playerTex);
	UnloadTexture(moonTex);
	UnloadTexture(spaceTex);
}