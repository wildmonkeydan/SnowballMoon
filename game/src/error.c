#include <raylib.h>
#include "error.h"

void ErrorTrap(ErrorType type, const char* message) {
	ToggleBorderlessWindowed();
	SetWindowSize(640, 240);
	SetWindowPosition((GetMonitorWidth(GetCurrentMonitor()) / 2) - 320, (GetMonitorHeight(GetCurrentMonitor()) / 2) - 240);
	

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(BLACK);

		DrawText("ERROR", 270, 10, 20, RED);
		DrawText(message, 10, 128, 12, WHITE);

		EndDrawing();
	}
}