#pragma once
#include "Enemy.h"
#include<vector>


class EnemyMgr {
public:
	static void init();
	static void spawnEnemy(EnemyType type, glm::vec3 pos);
	static void checkDeadEnemy();
	static void deleteAllEnemies();

	static inline glm::vec3* dronePos = nullptr;
private:
	static void deleteEnemy(Enemy* enemy);

	static inline std::vector<Enemy*> allEnemies;
};