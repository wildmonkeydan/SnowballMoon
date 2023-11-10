#include <raylib.h>
#include "player.h"

void GameLoop() {
	// Init vars

	float moonRadius = GetScreenHeight() / 2.f - (GetScreenHeight() * 0.1f);
	Vector2 moonCenter = {  GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };

	Texture2D playerTex = LoadTexture("test.png");

	Player mainPlayer;
	CreatePlayer(&mainPlayer, 0);

	while (!WindowShouldClose()) {
		UpdatePlayer(&mainPlayer, GetFrameTime());

		BeginDrawing();

		ClearBackground(BLACK);

		DrawCircle(moonCenter.x, moonCenter.y, moonRadius, RAYWHITE);
		DrawPlayer(&mainPlayer, moonCenter, moonRadius, playerTex);

		EndDrawing();
	}

	UnloadTexture(playerTex);
}