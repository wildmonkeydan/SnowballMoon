#pragma once
#include <raylib.h>
#include "types.h"

/// <summary>
/// Animate a sprite with a pre-defined animation
/// </summary>
/// <param name="anim:">
/// DefinedAnimation to animate with
/// </param>
/// <param name="ctx:">
/// AnimationContext to use
/// </param>
/// <param name="delta:">
/// Time taken for the previous frame
/// </param>
/// <param name="sounds:">
/// Array of sound effects
/// </param>
/// <returns>
/// UV co-ordinates for the current frame
/// </returns>
Vector2 animation_AnimateDef(DefinedAnimations anim, AnimContext* ctx, float delta, Sound* sounds);

/// <summary>
/// Clear an AnimationContext (restart an animation)
/// </summary>
/// <param name="ctx:">
/// AnimationContext to clear
/// </param>
void animation_ClearContext(AnimContext* ctx);