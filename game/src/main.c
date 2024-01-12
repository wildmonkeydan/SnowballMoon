#include <raylib.h>
#include "game.h"
#include "JoyShock/JoyShockLibrary.h"

int main() {
	void Init();
	void Deinit();
	void MMLogo();

	Init();

	MMLogo();
	GameLoop();

	Deinit();

	return 0;
}

void Init() {
	InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Snowball Moon");
	InitAudioDevice();
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
	ToggleBorderlessWindowed();
	SetRandomSeed(GetTime());
}

void Deinit() {
	JslDisconnectAndDisposeAll();
	CloseAudioDevice();
	CloseWindow();
}

void MMLogo() {
	Texture logo = LoadTexture("Data/MMLogo.png");
	SetTextureFilter(logo, TEXTURE_FILTER_TRILINEAR);

	if (logo.id == NULL) {
		return;
	}

	int width = (GetScreenHeight() / 9.f) * 16;
	float timer = 0.f;
	float fade = 0.f;

	while (!WindowShouldClose()) {
		timer += GetFrameTime();

		if (timer > 4.f)
			break;

		if (timer > 3.f) {
			fade = timer - 3.f;
		}

		BeginDrawing();

		ClearBackground(BLACK);

		DrawTexturePro(logo, 
			(Rectangle) { 0, 0, logo.width, logo.height },
			(Rectangle) { GetScreenWidth() * 0.625f, 0,  width, GetScreenHeight() },
			(Vector2) { logo.width / 2.f, 0 }, 0.f, ColorAlpha(RAYWHITE, 1.f - fade));

		EndDrawing();
	}

	UnloadTexture(logo);
}