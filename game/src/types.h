#pragma once
#include <raylib.h>

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

typedef enum {
	PS_STAND,
	PS_CROUCH,
	PS_COLLECT,
	PS_THROW,
	PS_RUN,
	PS_HIT
}PlayerState;

typedef struct {
	float angle;
	PlayerState state;
	PlayerState prevState;
	Rectangle collision;
	float stateTimer;
	Color colour;
	bool hasSnowball;
	bool flipped;
	int score;
	int id;
	AnimContext ctx;
}Player;

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

