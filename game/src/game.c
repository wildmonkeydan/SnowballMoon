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

	float moonRadius = GetScreenHeight() / 2.f - (GetScreenHeight() * 0.1f);
	Vector2 moonCenter = {  GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };

	Texture2D playerTex = LoadTexture("PlayerSprites.png");
	SetTextureFilter(playerTex, TEXTURE_FILTER_POINT);

	Player mainPlayer;
	CreatePlayer(&mainPlayer, 0);

	Player secondPlayer;
	CreatePlayer(&secondPlayer, 1);

	secondPlayer.angle = 30;
	secondPlayer.colour = GREEN;

	while (!WindowShouldClose()) {
		float delta = GetFrameTime();

		nextSnowball = UpdatePlayer(&mainPlayer, delta, snowballs, nextSnowball);
		nextSnowball = UpdatePlayer(&secondPlayer, delta, snowballs, nextSnowball);

		for (int i = 0; i < MAX_SNOWBALLS; i++) {
			UpdateSnowball(&snowballs[i], delta);
		}

		BeginDrawing();

		ClearBackground(BLACK);

		DrawCircle(moonCenter.x, moonCenter.y, moonRadius, RAYWHITE);
		DrawPlayer(&mainPlayer, moonCenter, moonRadius, playerTex, delta);
		DrawPlayer(&secondPlayer, moonCenter, moonRadius, playerTex, delta);

		for (int i = 0; i < MAX_SNOWBALLS; i++) {
			DrawSnowball(&snowballs[i], moonCenter, moonRadius);
		}

		EndDrawing();
	}

	UnloadTexture(playerTex);
}