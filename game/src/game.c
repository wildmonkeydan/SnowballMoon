#include <raylib.h>
#include "player.h"
#include "snowball.h"

#define MAX_SNOWBALLS 256

void GameLoop() {
	// Init vars

	Snowball snowballs[MAX_SNOWBALLS];
	int nextSnowball = 0;

	for (int i = 0; i < MAX_SNOWBALLS; i++) {
		snowballs[i].active = false;
	}

	float moonRadius = GetScreenHeight() / 2.f - (GetScreenHeight() * 0.15f);
	Vector2 moonCenter = {  GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };
	int moonRadiusAdjust = GetScreenWidth() / 384;
	int playerSize = moonRadius / 5;

	Texture2D playerTex = LoadTexture("PlayerSprites.png");
	SetTextureFilter(playerTex, TEXTURE_FILTER_TRILINEAR);

	Texture2D moonTex = LoadTexture("Moon.png");
	SetTextureFilter(moonTex, TEXTURE_FILTER_TRILINEAR);

	Texture2D spaceTex = LoadTexture("Space.png");
	SetTextureFilter(spaceTex, TEXTURE_FILTER_TRILINEAR);
	SetTextureWrap(spaceTex, TEXTURE_WRAP_REPEAT);

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

		nextSnowball = UpdatePlayer(&players[0], playerSize, delta, snowballs, nextSnowball);
		nextSnowball = UpdatePlayer(&players[1], playerSize, delta, snowballs, nextSnowball);
		nextSnowball = UpdatePlayer(&players[2], playerSize, delta, snowballs, nextSnowball);
		nextSnowball = UpdatePlayer(&players[3], playerSize, delta, snowballs, nextSnowball);

		for (int i = 0; i < MAX_SNOWBALLS; i++) {
			UpdateSnowball(&snowballs[i], players, 4, playerSize, moonRadius - moonRadiusAdjust, moonCenter, delta);
		}

		BeginDrawing();

		ClearBackground(BLACK);

		DrawTextureRec(spaceTex, (Rectangle) { 0, 0, GetScreenWidth(), GetScreenHeight() }, (Vector2) { 0, 0 }, RAYWHITE);

		DrawTexturePro(moonTex, (Rectangle) { 0, 0, moonTex.width, moonTex.height }, (Rectangle) { moonCenter.x - moonRadius, moonCenter.y - moonRadius, moonRadius * 2, moonRadius * 2 }, (Vector2) { 0, 0 }, 0.f, RAYWHITE);
		//DrawCircle(moonCenter.x, moonCenter.y, moonRadius, RAYWHITE);
		DrawPlayer(&players[0], moonCenter, moonRadius - moonRadiusAdjust, playerTex, playerSize, delta);
		DrawPlayer(&players[1], moonCenter, moonRadius - moonRadiusAdjust, playerTex, playerSize, delta);
		DrawPlayer(&players[2], moonCenter, moonRadius - moonRadiusAdjust, playerTex, playerSize, delta);
		DrawPlayer(&players[3], moonCenter, moonRadius - moonRadiusAdjust, playerTex, playerSize, delta);

		for (int i = 0; i < MAX_SNOWBALLS; i++) {
			DrawSnowball(&snowballs[i], moonCenter, moonRadius - moonRadiusAdjust);
		}

		DrawText(TextFormat("%d", players[0].score), 0, 0, 30, players[0].colour);
		DrawText(TextFormat("%d", players[1].score), 1800, 0, 30, players[1].colour);
		DrawText(TextFormat("%d", players[2].score), 0, 900, 30, players[2].colour);
		DrawText(TextFormat("%d", players[3].score), 1800, 900, 30, players[3].colour);

		EndDrawing();
	}

	UnloadTexture(playerTex);
}