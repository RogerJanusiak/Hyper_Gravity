#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include "../HEng/Level.h"
#include "../HEng/State.h"

enum ENEMY_TYPE {
	robor,
	roborto,
	robro,
	romo,
	roo
};

class Enemy {

public:
	virtual ~Enemy() = default;
	explicit Enemy(Entity* _entity);

	virtual void move(GlobalGameState& ggs, const std::list<Platform> &platforms, Level& level);
	virtual int getDifficulty() { return 1; }
	virtual ENEMY_TYPE getType() { return robor; }

	void render() const;
	[[nodiscard]] Entity* getEntity() const { return entity; }

	static int findEdgeRight(int startX, int startY,  Level& level);
	static int findEdgeLeft(int startX, int startY, Level& level);

	[[nodiscard]] virtual float getXVelocity() const { return scale(125); }

	[[nodiscard]] virtual int getWidth() const { return scale(16); }
	[[nodiscard]] virtual int getHeight() const { return scale(30); }

protected:
	Entity* entity;
	double spawnTimer = 0;
	double spawnDisplayTime = 0;
	bool firstSpawnLoop = true;

};


#endif //ENEMY_H
