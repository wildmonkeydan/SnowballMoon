#include "animation.h"

// Defined Animations

Animation playerIdle = { 6, 3, 2 };
Animation playerRun = { 30, 14, 0 };
Animation playerCrouch = { 24, 8, 1 };
Animation playerThrow = { 24, 21, 3 };
Animation playerHitFront = { 24, 18, 4 };
Animation playerPickup = { 12, 13, 5 };

Vector2 animation_AnimateDef(DefinedAnimations anim, AnimContext* ctx, float delta) {
	Animation* ref;

	switch (anim) {
	case DA_PLAYERIDLE:
		ref = &playerIdle;
		break;
	case DA_PLAYERRUN:
		ref = &playerRun;
		break;
	case DA_PLAYERCROUCH:
		ref = &playerCrouch;
		break;
	case DA_THROW:
		ref = &playerThrow;
		break;
	case DA_HITFRONT:
		ref = &playerHitFront;
		break;
	case DA_PICKUP:
		ref = &playerPickup;
		break;
	default:
		ref = &playerIdle;
		break;
	}

	Vector2 uvCoords = { ctx->currentFrame * 128, ref->animLane * 128 };

	if (ctx->currentFrame < ref->length) {
		if (ctx->timer >= (1 / (float)ref->fps)) {
			ctx->currentFrame++;
			ctx->timer = 0;
		}
		else
		{
			ctx->timer += delta;
		}
	}
	else {
		if (ctx->loop) {
			ctx->currentFrame = 0;
		}
	}
	
	return uvCoords;
}

void animation_ClearContext(AnimContext* ctx) {
	ctx->currentFrame = 0;
	ctx->timer = 0;
}