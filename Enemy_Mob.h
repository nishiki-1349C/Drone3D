#pragma once
#include "Enemy.h"


class Mob : public Enemy {
public:
	Mob(glm::vec3 pos);

	void update() override;
	void onCollision(Object* other) override;

private:
	enum State {
		idle,
		chase,
		attack,
		dead
	};

	glm::vec3 velocity = glm::vec3(0);
	State state = idle;

	//state = idle
	float idleTime = 3.0f;
	float idleTimer = idleTime;
	float detectionRange = 13.0f;

	//state = chase
	float chaseSpeed = 5.0f;
	glm::vec2 attackDir = glm::vec2(0);

	//state = attack
	bool isAttacking = false;
	float attackRange = 4.0f;
	float attackTimer = 0.0f;
	float attackWaitTime = 1.0f;
	float jumpSpeed = 7.0f;
	float jumpTime = 1.15f;
	float coolTime = 2.0f;

	//state = dead
	float deadTime = 1.5, deadTimer = deadTime;
	glm::vec3 baseColor;
};