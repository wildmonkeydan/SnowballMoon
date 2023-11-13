#include "snowball.h"
#include "player.h"
#include <raylib.h>
#include <raymath.h>

#define SNOWBALL_SPEED 180.f
#define STRAIGHT_GRAVITY 160.f
#define SNOWBALL_RADIUS_SCALAR 0.01f 

void CreateSnowballStraight(Snowball* sb, int playerHeight, int owner, float angle, int direction) {
	sb->owner = owner;
	sb->type = SPT_STRAIGHT;
	sb->angle = angle;
	sb->direction = direction;
	sb->height = playerHeight * 0.75f;
	sb->active = true;
}

void CreateSnowballGravity(Snowball* sb, int owner, float angle, Vector2 moonMiddle, float moonRadius, Vector2 velocity) {
	sb->owner = owner;
	sb->type = SPT_GRAVITY;

	sb->position = Vector2Add((Vector2) { moonRadius* cosf(DEG2RAD * angle), moonRadius* sinf(DEG2RAD * angle) }, moonMiddle);
	sb->velocity = velocity;

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

			Vector2 playerCollision[4] = { 0 };

			for (int i = 0; i < numPlayers; i++) {
				// Rotate collision

				int x = (moonRadius + players[i].collision.y) * cosf(DEG2RAD * players[i].angle);
				int y = (moonRadius + players[i].collision.y) * sinf(DEG2RAD * players[i].angle);

				float sinRotation = sinf((players[i].angle + 90) * DEG2RAD);
				float cosRotation = cosf((players[i].angle + 90) * DEG2RAD);
				float dx = -playerSize / 4;
				float dy = -playerSize;

				playerCollision[0].x = x + dx * cosRotation - dy * sinRotation;
				playerCollision[0].y = y + dx * sinRotation + dy * cosRotation;

				playerCollision[1].x = x + dx * cosRotation - (dy + players[i].collision.height) * sinRotation;
				playerCollision[1].y = y + dx * sinRotation + (dy + players[i].collision.height) * cosRotation;


				playerCollision[2].x = x + (dx + players[i].collision.width) * cosRotation - (dy + players[i].collision.height) * sinRotation;
				playerCollision[2].y = y + (dx + players[i].collision.width) * sinRotation + (dy + players[i].collision.height) * cosRotation;


				playerCollision[3].x = x + (dx + players[i].collision.width) * cosRotation - dy * sinRotation;
				playerCollision[3].y = y + (dx + players[i].collision.width) * sinRotation + dy * cosRotation;

				for (int i = 0; i < 4; i++) {
					playerCollision[i] = Vector2Add(playerCollision[i], moonMiddle);
				}

				Vector2 snowballPos = (Vector2){ ((moonRadius + sb->height) * cosf(DEG2RAD * sb->angle)) + moonMiddle.x, ((moonRadius + sb->height) * sinf(DEG2RAD * sb->angle)) + moonMiddle.y };

				if (CheckCollisionPointPoly(snowballPos, playerCollision, 4) && ((players[i].angle + 30) > sb->angle && (players[i].angle - 30) < sb->angle)) {
					sb->active = false;

					if (sb->owner == players[i].id) { // If hit self
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