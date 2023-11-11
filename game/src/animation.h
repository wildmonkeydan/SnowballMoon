#pragma once
#include "raylib.h"

typedef enum {
	DA_PLAYERIDLE,
	DA_PLAYERRUN,
	DA_PLAYERCROUCH
}DefinedAnimations;

typedef struct AnimContext {
	unsigned char currentFrame;
	bool loop;
	float timer;
}AnimContext;

typedef struct Animation {
	unsigned char fps;
	unsigned char length;
	unsigned short animLane;
}Animation;

Vector2 animation_AnimateDef(DefinedAnimations anim, AnimContext* ctx, float delta);
void animation_ClearContext(AnimContext* ctx);