#include <raylib.h>
#include <raymath.h>
#include "types.h"
#include "animation.h"

void CreateFort(Fort* fort, int health, Color team, int id, Vector2 pos, int size, bool flip) {
	fort->health = health;
	fort->teamColour = team;
	fort->teamId = id;
	fort->flipped = flip;

	fort->pos.x = pos.x;
	fort->pos.y = pos.y;
	fort->pos.width = size;
	fort->pos.height = size;

	animation_ClearContext(&fort->ctx);
}

void DrawFort(Fort* fort, Texture2D fortTex, float delta) {

	Rectangle texRect = { 0 };
	
	if (fort->flipped) {
		texRect.height = -256; texRect.width = -256;
	}
	else {
		texRect.width = 256; texRect.height = 256;
	}
	
	if (fort->health > 0) {
		Vector2 uv = animation_AnimateDef(DA_FLAG, &fort->ctx, delta);

		texRect.x = uv.x;
		texRect.y = uv.y;
		DrawTexturePro(fortTex, texRect, fort->pos, (Vector2) { 0, 0 }, 0.f, fort->teamColour);
	}

	if (fort->health > 0) {
		texRect.x = 1920;
		texRect.y = 0;
	}
	else {
		texRect.x = 2176;
		texRect.y = 0;
	}

	DrawTexturePro(fortTex, texRect, fort->pos, Vector2Zero(), 0.f, RAYWHITE);

	
}