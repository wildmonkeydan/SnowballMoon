#pragma once
#include <raylib.h>

typedef enum {
	SPT_STRAIGHT,
	SPT_GRAVITY
}SnowballPhysicsType;

typedef struct {
	bool active;

	int owner;
	SnowballPhysicsType type;
	// Used for SPT_GRAVITY
	Vector2 position;
	Vector2 velocity;
	// Used for SPT_STRAIGHT
	float angle;
	float height;
	int direction;
}Snowball;

void CreateSnowballStraight(Snowball* sb, int owner, float angle, int direction);
void CreateSnowballGravity(Snowball* sb, int owner, float angle, Vector2 moonMiddle, float moonRadius, Vector2 velocity);
void UpdateSnowball(Snowball* sb, float delta);
void DrawSnowball(Snowball* sb, Vector2 moonMiddle, float moonRadius);