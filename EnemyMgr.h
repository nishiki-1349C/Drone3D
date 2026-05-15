#pragma once
#include "Enemy.h"
#include<vector>


class EnemyMgr {
public:
	static void spawnEnemy(EnemyType type, glm::vec3 pos);
	static void checkDeadEnemy();
	static void deleteAllEnemies();


private:
	static void deleteEnemy(Enemy* enemy);

	static std::vector<Enemy*> allEnemies;

};
