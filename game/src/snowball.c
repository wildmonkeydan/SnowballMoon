#include "snowball.h"
#include "player.h"
#include <raylib.h>
#include <raymath.h>


#define SNOWBALL_SPEED 180.f
#define STRAIGHT_GRAVITY 160.f
#define STANDARD_GRAVITY 4000
#define SNOWBALL_RADIUS_SCALAR 0.01f 

void CreateSnowballStraight(Snowball* sb, int playerHeight, int owner, float angle, int direction) {
	sb->owner = owner;
	sb->type = SPT_STRAIGHT;
	sb->angle = angle;
	sb->direction = direction;
	sb->height = playerHeight * 0.75f;
	sb->active = true;
}

void CreateSnowballGravity(Snowball* sb, int owner, float angle, Vector2 moonMiddle, float moonRadius, Vector2 direction, int playerHeight) {
	sb->owner = owner;
	sb->type = SPT_GRAVITY;

	sb->height = playerHeight * 0.75f;
	sb->position = Vector2Add((Vector2) { (moonRadius + sb->height) * cosf(DEG2RAD * angle) , (moonRadius + sb->height) * sinf(DEG2RAD * angle) }, moonMiddle);
	sb->velocity = direction;
	sb->vDirection = direction;

	sb->active = true;
}

void UpdateSnowball(Snowball* sb, Player* players, int numPlayers, int playerSize, float moonRadius, Vector2 moonMiddle, float delta) {
	if (sb->active) {

		if (sb->type == SPT_STRAIGHT) {

			sb->angle += (SNOWBALL_SPEED * delta) * sb->direction;
			sb->height -= STRAIGHT_GRAVITY * delta;

			if (sb->height <= 0) {
				sb->active = false;
			}

			for (int i = 0; i < numPlayers; i++) {
				Vector2 snowballPos = (Vector2){ ((moonRadius + sb->height) * cosf(DEG2RAD * sb->angle)) + moonMiddle.x, ((moonRadius + sb->height) * sinf(DEG2RAD * sb->angle)) + moonMiddle.y };


				if (CheckCollisionPointTriangle(snowballPos, players[i].collisionPoly[1], players[i].collisionPoly[0], players[i].collisionPoly[3])
					|| CheckCollisionPointTriangle(snowballPos, players[i].collisionPoly[3], players[i].collisionPoly[2], players[i].collisionPoly[1])) {
					sb->active = false;

					if (sb->owner == players[i].playerId) { // If hit self
						players[i].score--;
					}
					else {
						players[sb->owner].score++;
					}

					players[i].state = PS_HIT;
					players[i].stateTimer = 0;
				}
			}

			if (sb->angle >= 360) {
				sb->angle = 0;
			}
			if (sb->angle < 0) {
				sb->angle = 359;
			}

		}
		else {

			Vector2 gravity = { 0, 1.f };

			gravity = Vector2Subtract(moonMiddle, sb->position);
			gravity = Vector2Normalize(gravity);

			sb->velocity.x += (gravity.x * STANDARD_GRAVITY) * delta;
			sb->velocity.y += (gravity.y * STANDARD_GRAVITY) * delta;

			sb->velocity.x += sb->vDirection.x * delta;
			sb->velocity.y += sb->vDirection.y * delta;

			sb->position.x += sb->velocity.x * delta;
			sb->position.y += sb->velocity.y * delta;

			if (CheckCollisionPointCircle(sb->position, moonMiddle, moonRadius)) {
				sb->active = false;
			}
			else
			{
				for (int i = 0; i < numPlayers; i++) {

					if (CheckCollisionPointTriangle(sb->position, players[i].collisionPoly[1], players[i].collisionPoly[0], players[i].collisionPoly[3]) 
						|| CheckCollisionPointTriangle(sb->position, players[i].collisionPoly[3], players[i].collisionPoly[2], players[i].collisionPoly[1])) {
						sb->active = false;

						if (sb->owner == players[i].playerId) { // If hit self
							players[i].score--;
						}
						else {
							players[sb->owner].score++;
						}

						players[i].state = PS_HIT;
						players[i].stateTimer = 0;
					}
				}
			}
			
		}

	}
}

void DrawSnowball(Snowball* sb, Vector2 moonMiddle, float moonRadius) {
	if (sb->active) {

		if (sb->type == SPT_STRAIGHT) {
			DrawCircle(((moonRadius + sb->height) * cosf(DEG2RAD * sb->angle)) + moonMiddle.x, ((moonRadius + sb->height) * sinf(DEG2RAD * sb->angle)) + moonMiddle.y, moonRadius * SNOWBALL_RADIUS_SCALAR, RAYWHITE);
		}
		else {
			DrawCircle(sb->position.x, sb->position.y, moonRadius * SNOWBALL_RADIUS_SCALAR, RAYWHITE);
		}

	}
}