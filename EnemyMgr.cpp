#include "EnemyMgr.h"
#include "Enemy_Mob.h"
#include "MainDrone.h"
using namespace std;
using namespace glm;

// 実体定義
std::vector<Enemy*> EnemyMgr::allEnemies;
glm::vec3* EnemyMgr::dronePos = nullptr;

void EnemyMgr::init() {
	dronePos = &MainDrone::mainDrone->currentPos;
}

void EnemyMgr::spawnEnemy(EnemyType type, glm::vec3 pos) {
	Enemy* enemy = nullptr;
	switch ( type ) {
		case EnemyType::mob: enemy = new Mob(pos);
			break;
		default: break;
	}
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