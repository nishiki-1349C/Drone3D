#pragma once
#include "Object.h"
#include "EnemyData.h"

class Enemy : public Object {
public:
	Enemy(EnemyType type, glm::vec3 pos);
	virtual ~Enemy();

	virtual void update() override;
	virtual void onCollision(Object* other) override;

	bool isDead = false;
protected:
	int state, hp, attack;
	float speed;
	float enemyTimer = 0;
};

