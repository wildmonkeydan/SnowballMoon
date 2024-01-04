#include <raylib.h>
#include "types.h"

void CreateParticle(Particle* p, Vector2 pos);

void UpdateParticle(Particle* p, float delta);

void DrawParticle(Particle* p, float moonRadius);