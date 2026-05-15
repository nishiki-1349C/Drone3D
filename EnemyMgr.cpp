#include "EnemyMgr.h"
using namespace std;
using namespace glm;

std::vector<Enemy*> EnemyMgr::allEnemies; // ŽÀ‘Ì’è‹`

void EnemyMgr::spawnEnemy(EnemyType type, glm::vec3 pos) {
	Enemy* enemy = new Enemy(type, pos);
	allEnemies.push_back(enemy);
}

void EnemyMgr::checkDeadEnemy() {
	for ( auto it = allEnemies.begin(); it != allEnemies.end(); ) {
		if ( (*it)->isDead ) {
			delete* it;
			it = allEnemies.erase(it);
		}
		else { ++it; }
	}
}

void EnemyMgr::deleteEnemy(Enemy* enemy) {
	auto it = std::find(allEnemies.begin(), allEnemies.end(), enemy);
	if ( it != allEnemies.end() ) {
		delete* it;
		allEnemies.erase(it);
	}
}

void EnemyMgr::deleteAllEnemies() {
	for ( Enemy* enemy : allEnemies ) {
		delete enemy;
	}
	allEnemies.clear();
}