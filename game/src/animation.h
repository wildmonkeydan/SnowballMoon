#pragma once
#include <raylib.h>
#include "types.h"

Vector2 animation_AnimateDef(DefinedAnimations anim, AnimContext* ctx, float delta);
void animation_ClearContext(AnimContext* ctx);