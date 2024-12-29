#ifndef WAVE_H
#define WAVE_H

#include <memory>

#include "../Utils/Animations.h"
#include "../Entities/Enemy.h"
#include "Level.h"
#include "../Entities/Player.h"

class Wave {

public:

	explicit Wave(GlobalGameState& ggs, Player& timpy, Level& level, int waveNumber);

	bool runWave();
	void render();

	[[nodiscard]] std::list<Entity>& getBulletEntities() { return eBullets; }
	[[nodiscard]] std::list<Bullet>& getBullets() { return bullets; }
	[[nodiscard]] bool hasWaveStarted() const { return waveStarted; }

private:

	void createEnemies();

	GlobalGameState& ggs;
	Player& player;
	Level& level;
	int waveNumber = 0;

	int enemiesAlive = 0;

	bool waveStarted = false;
	float timeSinceLoad = 0;

	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::list<Entity*> allCharacterEntities;

	std::list<Entity> eBullets;
	std::list<Bullet> bullets;

	std::list<Explosion> explosions;

};

#endif
