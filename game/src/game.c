#include <raylib.h>
#include "player.h"

void GameLoop() {
	// Init vars

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

		UpdatePlayer(&mainPlayer, delta);
		UpdatePlayer(&secondPlayer, delta);

		BeginDrawing();

		ClearBackground(BLACK);

		DrawCircle(moonCenter.x, moonCenter.y, moonRadius, RAYWHITE);
		DrawPlayer(&mainPlayer, moonCenter, moonRadius, playerTex, delta);
		DrawPlayer(&secondPlayer, moonCenter, moonRadius, playerTex, delta);

		EndDrawing();
	}

	UnloadTexture(playerTex);
}