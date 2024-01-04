#include "particle.h"

#define PARTICLE_LIFETIME 1.f
#define PARTICLE_RADIUS_SCALAR 0.008f 

void CreateParticle(Particle* p, Vector2 pos) {
	p->active = true;
	p->lifeTimer = 0.f;
	p->position = pos;
}

void UpdateParticle(Particle* p, float delta) {
	p->lifeTimer += delta;

	if (p->lifeTimer >= PARTICLE_LIFETIME) {
		p->active = false;
	}
}

void DrawParticle(Particle* p, float moonRadius) {
	if (p->active) {
		float size = moonRadius * PARTICLE_RADIUS_SCALAR;

		if (size < 1) {
			size = 1; // Make sure the particles always show, even on lower resolutions
		}

		DrawCircleV(p->position, size, ColorAlpha(RAYWHITE, PARTICLE_LIFETIME - p->lifeTimer));
	}
}