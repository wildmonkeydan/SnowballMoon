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
	DA_PICKUP,
	DA_FLAG
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
	PS_HIT,
	PS_AIM,
	PS_THROW_GRAVITY
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
	unsigned char snowballCount;
	bool throwing;
	float snowballAngle;
	bool flipped;
	int score;
	int id; // Input ID
	int playerId; // Player ID
	unsigned char team;
	AnimContext ctx;
	Vector2 collisionPoly[4];
}Player;

#define PLAYER_COLOUR_VIOLET	PURPLE			// Player Palette - Violet
#define PLAYER_COLOUR_NAVY		DARKBLUE		// Player Palette - Navy
#define PLAYER_COLOUR_BROWN		RAYWHITE		// Player Palette - Brown
#define PLAYER_COLOUR_GREEN		GREEN			// Player Palette - Green
#define PLAYER_COLOUR_PINK		PINK			// Player Palette - Pink
#define PLAYER_COLOUR_BLUE		BLUE			// Player Palette - Blue
#define PLAYER_COLOUR_ORANGE	ORANGE			// Player Palette - Orange
#define PLAYER_COLOUR_LIME		DARKGREEN		// Player Palette - Lime


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
	float particleTimer;


	// Used for SPT_GRAVITY
	Vector2 position;
	Vector2 velocity;
	Vector2 vDirection;
	// Used for SPT_STRAIGHT
	float angle;
	float height;
	int direction;
}Snowball;

/// <summary>
/// Particle data type, for snow trails
/// </summary>
typedef struct Particle {
	bool active;

	Vector2 position;
	float lifeTimer;
}Particle;

/// <summary>
/// Enum for game mode logic
/// </summary>
typedef enum GameMode {
	GM_FREE_FOR_ALL,
	GM_HOARDER,
	GM_TEAM_FORT
}GameMode;

/// <summary>
/// Fort data type, contains all relevant data
/// </summary>
typedef struct Fort {
	int health;
	int maxHealth;
	Color teamColour;
	int teamId;
	bool flipped;
	Rectangle pos;
	AnimContext ctx;
}Fort;

/// <summary>
/// Enum for music tracks in the game
/// </summary>
typedef enum MusicTrack {
	MUS_LOBBY,
	MUS_DIAMOND,
	MUS_ESKIMO,
	MUS_ARTIC
}MusicTrack;

/// <summary>
/// Enum for sound effects in the game
/// </summary>
typedef enum SoundEffect {
	SND_SCROLL,
	SND_CONFIRM,
	SND_STEP0,
	SND_STEP1,
	SND_STEP2,
	SND_STEP3,
	SND_HIT,
	SND_THROW,
	SND_THUD,
	SND_FORM
};