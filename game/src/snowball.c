#include "snowball.h"
#include <raymath.h>

#define SNOWBALL_SPEED 100.f
#define STRAIGHT_GRAVITY 40.f
#define SNOWBALL_RADIUS_SCALAR 0.01f 

void CreateSnowballStraight(Snowball* sb, int owner, float angle, int direction) {
	sb->owner = owner;
	sb->type = SPT_STRAIGHT;
	sb->angle = angle;
	sb->direction = direction;
	sb->height = 90.f;
	sb->active = true;
}

void CreateSnowballGravity(Snowball* sb, int owner, float angle, Vector2 moonMiddle, float moonRadius, Vector2 velocity) {
	sb->owner = owner;
	sb->type = SPT_GRAVITY;

	sb->position = Vector2Add((Vector2) { moonRadius* cosf(DEG2RAD * angle), moonRadius* sinf(DEG2RAD * angle) }, moonMiddle);
	sb->velocity = velocity;

	sb->active = true;
}

void UpdateSnowball(Snowball* sb, float delta) {
	if (sb->active) {

		if (sb->type == SPT_STRAIGHT) {

			sb->angle += (SNOWBALL_SPEED * delta) * sb->direction;
			sb->height -= STRAIGHT_GRAVITY * delta;

			if (sb->height <= 0) {
				sb->active = false;
			}

		}
		else {

		}

	}
}

void DrawSnowball(Snowball* sb, Vector2 moonMiddle, float moonRadius) {
	if (sb->active) {

		if (sb->type == SPT_STRAIGHT) {
			DrawCircle(((moonRadius + sb->height) * cosf(DEG2RAD * sb->angle)) + moonMiddle.x, ((moonRadius + sb->height) * sinf(DEG2RAD * sb->angle)) + moonMiddle.y, moonRadius * SNOWBALL_RADIUS_SCALAR, RAYWHITE);
		}
		else {

		}

	}
}