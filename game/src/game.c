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

	Texture2D playerTex = LoadTexture("PlayerSprites.png");
	SetTextureFilter(playerTex, TEXTURE_FILTER_TRILINEAR);

	Texture2D moonTex = LoadTexture("Moon.png");
	SetTextureFilter(moonTex, TEXTURE_FILTER_TRILINEAR);

	Player players[2];
	CreatePlayer(&players[0], 0);
	CreatePlayer(&players[1], 1);

	players[1].angle = 30;
	players[1].colour = GREEN;

	while (!WindowShouldClose()) {
		float delta = GetFrameTime();

		nextSnowball = UpdatePlayer(&players[0], delta, snowballs, nextSnowball);
		nextSnowball = UpdatePlayer(&players[1], delta, snowballs, nextSnowball);

		for (int i = 0; i < MAX_SNOWBALLS; i++) {
			UpdateSnowball(&snowballs[i], players, 2, moonRadius - moonRadiusAdjust, moonCenter, delta);
		}

		BeginDrawing();

		ClearBackground(BLACK);

		DrawTexturePro(moonTex, (Rectangle) { 0, 0, moonTex.width, moonTex.height }, (Rectangle) { moonCenter.x - moonRadius, moonCenter.y - moonRadius, moonRadius * 2, moonRadius * 2 }, (Vector2) { 0, 0 }, 0.f, RAYWHITE);
		//DrawCircle(moonCenter.x, moonCenter.y, moonRadius, RAYWHITE);
		DrawPlayer(&players[0], moonCenter, moonRadius - moonRadiusAdjust, playerTex, delta);
		DrawPlayer(&players[1], moonCenter, moonRadius - moonRadiusAdjust, playerTex, delta);

		for (int i = 0; i < MAX_SNOWBALLS; i++) {
			DrawSnowball(&snowballs[i], moonCenter, moonRadius - moonRadiusAdjust);
		}

		DrawText(TextFormat("%d  %f", players[0].score, players[0].angle), 0, 0, 30, players[0].colour);
		DrawText(TextFormat("%d  %f", players[1].score, players[1].angle), 1200, 0, 30, players[1].colour);

		EndDrawing();
	}

	UnloadTexture(playerTex);
}