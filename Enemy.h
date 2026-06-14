#pragma once
#include "Object.h"
#include "EnemyData.h"
#include "MainDrone.h"

class Enemy : public Object {
public:
	Enemy(EnemyType type, glm::vec3 pos);
	//virtual ~Enemy();

	//virtual void update() override;
	virtual void onCollision(Object* other) override {};

	bool isDead = false;
protected:
	int hp, attack;
	float enemyTimer = 0;
};