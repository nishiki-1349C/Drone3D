#pragma once
#include "Enemy.h"
#include<vector>


class EnemyMgr {
public:
	static void init();
	static void spawnEnemy(EnemyType type, glm::vec3 pos);
	static void checkDeadEnemy();
	static void deleteAllEnemies();

	static glm::vec3* dronePos;
private:
	static void deleteEnemy(Enemy* enemy);

	static std::vector<Enemy*> allEnemies;
};