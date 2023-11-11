#pragma once
#include <raylib.h>

typedef enum {
	SPT_STRAIGHT,
	SPT_GRAVITY
}SnowballPhysicsType;

typedef struct {
	int owner;
	SnowballPhysicsType type;
	// Used for SPT_GRAVITY
	Vector2 position;
	Vector2 velocity;
	// Used for SPT_STRAIGHT
	float angle;
}Snowball;