#include <raylib.h>
#include "game.h"

int main() {
	void Init();
	void Deinit();

	Init();

	GameLoop();

	Deinit();

	return 0;
}

void Init() {
	InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Snowball Moon");
	InitAudioDevice();
}

void Deinit() {
	CloseAudioDevice();
	CloseWindow();
}