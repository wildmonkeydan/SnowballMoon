#include "animation.h"

// Defined Animations

Animation playerIdle = { 6, 3, 2 };
Animation playerRun = { 30, 14, 0 };
Animation playerCrouch = { 24, 8, 1 };
Animation playerThrow = { 24, 21, 3 };
Animation playerHitFront = { 24, 18, 4 };
Animation playerPickup = { 12, 13, 5 };
Animation fortFlag = { 15, 6, 6 };

Vector2 animation_AnimateDef(DefinedAnimations anim, AnimContext* ctx, float delta, Sound* sounds) {
	void DoSounds(AnimContext * ctx, DefinedAnimations anim, Sound * sounds);

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
	case DA_FLAG:
		ref = &fortFlag;
		break;
	default:
		ref = &playerIdle;
		break;
	}

	Vector2 uvCoords = { ctx->currentFrame * 128, ref->animLane * 128 };

	if (ref->animLane == 6) {
		uvCoords.x = ctx->currentFrame * 256;
	}

	

	if (ctx->currentFrame < ref->length) {
		if (ctx->timer >= (1 / (float)ref->fps)) {
			if (sounds != NULL) {
				DoSounds(ctx, anim, sounds);
			}
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

/// <summary>
/// Play sound effects with the animation
/// </summary>
/// <param name="ctx:">
/// AnimationContext to use
/// </param>
/// <param name="anim:">
/// DefinedAnimation to animate with
/// </param>
/// <param name="sounds:">
/// Array of sound effects
/// </param>
void DoSounds(AnimContext* ctx, DefinedAnimations anim, Sound* sounds) {
	switch (anim) {
	case DA_PLAYERRUN:
		if (ctx->currentFrame == 2 || ctx->currentFrame == 14) {
			PlaySound(sounds[SND_STEP0 + GetRandomValue(0, 3)]);
		}
		break;
	case DA_HITFRONT:
		if (ctx->currentFrame == 0) {
			PlaySound(sounds[SND_HIT]);
		}
		if (ctx->currentFrame == 7) {
			PlaySound(sounds[SND_THUD]);
		}
		break;
	case DA_THROW:
		if (ctx->currentFrame == 11) {
			PlaySound(sounds[SND_THROW]);
		}
		break;
	case DA_PICKUP:
		if (ctx->currentFrame == 2) {
			PlaySound(sounds[SND_FORM]);
		}
		break;
	default:
		break;
	}
}