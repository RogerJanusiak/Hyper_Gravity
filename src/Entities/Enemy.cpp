#include "../../includes/Entities/Enemy.h"

Enemy::Enemy(Entity *_entity) {
	entity = _entity;
}

void Enemy::move(GlobalGameState& ggs, const std::list<Platform> &platforms, Level& level) {
	if(entity->justSpawned) {
		entity->justSpawned = false;
		int direction = rand() % 2;
		if(direction == 0) {
			entity->setXVelocity(getXVelocity());
		} else {
			entity->setXVelocity(-getXVelocity());
		}
	} else {
		entity->move(ggs.dt,platforms);
	}
}

int Enemy::findEdgeRight(int startX, int startY, Level& level) {
	bool lookingRight = true;
	int numberTilesRight = 1;
	while(lookingRight) {
		int rightProbe = startX+numberTilesRight;
		if(rightProbe > 16) {
			numberTilesRight = -1;
			lookingRight = false;
		} else {
			if(level.getMap()[startY][rightProbe] != -1) {
				numberTilesRight++;
			} else {
				lookingRight = false;
			}
		}
	}
	return numberTilesRight;
}

int Enemy::findEdgeLeft(int startX, int startY, Level& level) {
	bool lookingLeft = true;
	int numberTilesLeft = 1;
	while(lookingLeft) {
		int leftProbe = startX-numberTilesLeft;
		if(leftProbe < 0) {
			numberTilesLeft = -1;
			lookingLeft = false;
		} else {
			if(level.getMap()[startY][leftProbe] != -1) {
				numberTilesLeft++;
			} else {
				lookingLeft = false;
			}
		}
	}
	return numberTilesLeft;
}