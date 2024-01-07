#include "snowball.h"
#include "player.h"
#include "particle.h"
#include <raylib.h>
#include <raymath.h>


#define SNOWBALL_SPEED 180.f
#define STRAIGHT_GRAVITY 160.f
#define STANDARD_GRAVITY 4000
#define SNOWBALL_RADIUS_SCALAR 0.01f 
#define PARTICLE_FREQUENCY 0.125f

void CreateSnowballStraight(Snowball* sb, int playerHeight, int owner, float angle, int direction) {
	sb->owner = owner;
	sb->type = SPT_STRAIGHT;
	sb->angle = angle;
	sb->direction = direction;
	sb->height = playerHeight * 0.75f;
	sb->active = true;
	sb->particleTimer = 0.f;
}

void CreateSnowballGravity(Snowball* sb, int owner, float angle, Vector2 moonMiddle, float moonRadius, Vector2 direction, int playerHeight) {
	sb->owner = owner;
	sb->type = SPT_GRAVITY;

	sb->height = playerHeight * 0.75f;
	sb->position = Vector2Add((Vector2) { (moonRadius + sb->height) * cosf(DEG2RAD * angle) , (moonRadius + sb->height) * sinf(DEG2RAD * angle) }, moonMiddle);
	
	sb->vDirection = Vector2Normalize(direction);
	sb->velocity.x = sb->vDirection.x * 2000.f;
	sb->velocity.y = sb->vDirection.y * 2000.f;

	sb->active = true;
	sb->particleTimer = 0.f;
}

int UpdateSnowball(Snowball* sb, Player* players, Fort* forts, Particle* particles, int nextParticle, GameMode mode, int numPlayers, int playerSize, float moonRadius, Vector2 moonMiddle, float delta) {
	if (sb->active) {
		sb->particleTimer += delta;

		if (sb->type == SPT_STRAIGHT) {

			sb->angle += (SNOWBALL_SPEED * delta) * sb->direction;
			sb->height -= STRAIGHT_GRAVITY * delta;

			if (sb->height <= 0) {
				sb->active = false;
			}

			Vector2 snowballPos = (Vector2){ ((moonRadius + sb->height) * cosf(DEG2RAD * sb->angle)) + moonMiddle.x, ((moonRadius + sb->height) * sinf(DEG2RAD * sb->angle)) + moonMiddle.y };


			for (int i = 0; i < numPlayers; i++) {
	
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

			if (mode == GM_TEAM_FORT) {

				for (int i = 0; i < 2; i++) {
					Rectangle fortCol = { 0 };

					if (forts[i].pos.height > 0) {
						fortCol = forts[i].pos;
					}
					else {
						fortCol.x = forts[i].pos.x;
						fortCol.y = forts[i].pos.y + forts[i].pos.height;

						fortCol.height = -forts[i].pos.height;
						fortCol.width = -forts[i].pos.height;
					}

					if (CheckCollisionPointRec(snowballPos, fortCol) && forts->health > 0) {
						sb->active = false;

						if (forts[i].teamId == players[sb->owner].team) {
							forts[i].health++;
							
							if (forts[i].health > forts[i].maxHealth) {
								forts[i].health = forts[i].maxHealth;
							}
						}
						else {
							forts[i].health--;
						}
					}
				}
			}

			if (sb->angle >= 360) {
				sb->angle = 0;
			}
			if (sb->angle < 0) {
				sb->angle = 359;
			}

			if (sb->particleTimer > PARTICLE_FREQUENCY) {
				CreateParticle(&particles[nextParticle], snowballPos);
				nextParticle++;

				if (nextParticle > 63) {
					nextParticle = 0;
				}

				sb->particleTimer = 0.f;
			}
		}
		else {

			Vector2 gravity = { 0, 1.f };

			gravity = Vector2Subtract(moonMiddle, sb->position);
			float distance = Vector2Length(gravity);
			gravity = Vector2Normalize(gravity);
			float distanceMultiplier = sqrtf(moonRadius/distance);

			sb->velocity.x += (gravity.x * (distanceMultiplier * STANDARD_GRAVITY)) * delta;
			sb->velocity.y += (gravity.y * (distanceMultiplier * STANDARD_GRAVITY)) * delta;

			sb->vDirection = Vector2Normalize(sb->velocity);

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

				if (mode == GM_TEAM_FORT) {

					for (int i = 0; i < 2; i++) {
						Rectangle fortCol = { 0 };

						if (forts[i].pos.height > 0) {
							fortCol = forts[i].pos;
						}
						else {
							fortCol.x = forts[i].pos.x;
							fortCol.y = forts[i].pos.y + forts[i].pos.height;

							fortCol.height = -forts[i].pos.height;
							fortCol.width = -forts[i].pos.height;
						}

						if (CheckCollisionPointRec(sb->position, fortCol) && forts->health > 0) {
							sb->active = false;

							if (forts[i].teamId == players[sb->owner].team) {
								forts[i].health++;

								if (forts[i].health > forts[i].maxHealth) {
									forts[i].health = forts[i].maxHealth;
								}
							}
							else {
								forts[i].health--;
							}
						}
					}
				}
			}
			if (sb->particleTimer > PARTICLE_FREQUENCY) {
				CreateParticle(&particles[nextParticle], sb->position);
				nextParticle++;

				if (nextParticle > 63) {
					nextParticle = 0;
				}

				sb->particleTimer = 0.f;
			}
		}
	}

	return nextParticle;
}

void DrawSnowball(Snowball* sb, Vector2 moonMiddle, float moonRadius) {
	if (sb->active) {

		float size = moonRadius * SNOWBALL_RADIUS_SCALAR;

		if (size < 1) {
			size = 1; // Make sure the snowballs appear, even on lower resolutions
		}

		if (sb->type == SPT_STRAIGHT) {
			DrawCircle(((moonRadius + sb->height) * cosf(DEG2RAD * sb->angle)) + moonMiddle.x, ((moonRadius + sb->height) * sinf(DEG2RAD * sb->angle)) + moonMiddle.y, size, RAYWHITE);
		}
		else {
			DrawCircle(sb->position.x, sb->position.y, size, RAYWHITE);
		}
	}
}