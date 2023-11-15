#pragma once
#include <raylib.h>


/// Animation Types

/// <summary>
/// Pre-defined animations
/// </summary>
typedef enum DefinedAnimations{
	DA_PLAYERIDLE,
	DA_PLAYERRUN,
	DA_PLAYERCROUCH,
	DA_THROW,
	DA_HITFRONT,
	DA_PICKUP
}DefinedAnimations;

/// <summary>
/// Context for animations, containing relevent data for animations
/// </summary>
typedef struct AnimContext {
	unsigned char currentFrame;
	bool loop;
	float timer;
}AnimContext;

/// <summary>
/// Animation definition, containing data for animation playback
/// </summary>
typedef struct Animation {
	unsigned char fps;
	unsigned char length;
	unsigned short animLane;
}Animation;


/// Player Types

/// <summary>
/// Enum for the player state machine
/// </summary>
typedef enum PlayerState{
	PS_STAND,
	PS_CROUCH,
	PS_COLLECT,
	PS_THROW,
	PS_RUN,
	PS_HIT
}PlayerState;

/// <summary>
/// Player data type, contains all relevant player data
/// </summary>
typedef struct Player{
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

#define PLAYER_COLOUR_VIOLET	CLITERAL(Color){ 68, 36, 52, 255 }
#define PLAYER_COLOUR_NAVY		CLITERAL(Color){ 48, 52, 109, 255 }


/// Snowball types

/// <summary>
/// Enum for snowball physics types
/// </summary>
typedef enum SnowballPhysicsType{
	SPT_STRAIGHT,
	SPT_GRAVITY
}SnowballPhysicsType;

/// <summary>
/// Snowball data type, conatins all relevant data and type
/// </summary>
typedef struct Snowball{
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