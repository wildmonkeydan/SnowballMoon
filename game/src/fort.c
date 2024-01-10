#include <raylib.h>
#include <raymath.h>
#include "types.h"
#include "animation.h"

void CreateFort(Fort* fort, int health, Color team, int id, Vector2 pos, int size, bool flip) {
	fort->health = health;
	fort->maxHealth = health;
	fort->teamColour = team;
	fort->teamId = id;
	fort->flipped = flip;

	fort->pos.x = pos.x;
	fort->pos.y = pos.y;
	fort->pos.width = size;
	fort->pos.height = size;

	animation_ClearContext(&fort->ctx);
}

void DrawFort(Fort* fort, Texture2D fortTex, float delta, int fontSize) {

	Rectangle texRect = { 0 };
	
	if (fort->flipped) {
		texRect.height = -256; texRect.width = -256;
	}
	else {
		texRect.width = 256; texRect.height = 256;
	}
	
	if (fort->health > 0) {
		Vector2 uv = animation_AnimateDef(DA_FLAG, &fort->ctx, delta, NULL);

		texRect.x = uv.x;
		texRect.y = uv.y;

		Rectangle dstRect = fort->pos;

		dstRect.y += fort->flipped ? (dstRect.height / 4) : -(dstRect.height / 4);

		DrawTexturePro(fortTex, texRect, dstRect, (Vector2) { 0, 0 }, 0.f, fort->teamColour);
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

	DrawTextPro(GetFontDefault(), TextFormat("%d", fort->health), 
		(Vector2) { fort->pos.x + fort->pos.height / 2, fort->pos.y + (fort->flipped ? (fort->pos.height / 3) : (fort->pos.height * 0.66f)) }, 
		(Vector2) { (TextLength(TextFormat("%d", fort->health)) * fontSize) / 4, 0 }, 
		fort->flipped ? 180 : 0, fontSize, fontSize / 10, fort->teamColour);
}