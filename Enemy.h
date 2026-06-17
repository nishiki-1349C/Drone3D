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
	void updateDroneDir(int perSec);		// 1秒あたりperSec回 droneDir を更新
	float dirTimer = 0.0f;					// 前回更新からの経過時間

	int hp, attack;
	glm::vec3 droneDir = glm::vec3(0.0f);	// 自分→droneの方向ベクトル
};