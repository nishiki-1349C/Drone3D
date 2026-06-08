#pragma once
#include "Enemy.h"

class Mob : public Enemy {
public:
	Mob(glm::vec3 pos);

	void update() override;
	void onCollision(Object* other) override;
};