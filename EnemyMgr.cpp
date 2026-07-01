#include "EnemyMgr.h"
#include "Enemy_Mob.h"
#include "Enemy_Tower.h"
using namespace std;
using namespace glm;


void EnemyMgr::setInstance(int stageNum) {
	switch ( stageNum ) {
		case 1:
			spawnEnemy(EnemyType::mob, vec3(25, 0, 25));
			spawnEnemy(EnemyType::mob, vec3(-20, 0, 25));
			spawnEnemy(EnemyType::mob, vec3(25, 0, 20));
			spawnEnemy(EnemyType::mob, vec3(-15, 0, 20));
			spawnEnemy(EnemyType::tower, vec3(0, 0, 50));
			break;
	}
}

void EnemyMgr::spawnEnemy(EnemyType type, glm::vec3 pos) {
	Enemy* enemy = nullptr;
	switch ( type ) {
		case EnemyType::mob:    enemy = new Mob(pos);    break;
		case EnemyType::tower:  enemy = new Tower(pos);  break;
		default: break;
	}
	if ( enemy ) {
		enemy->initRenderData();	// 生成時に描画データを構築（リセット時も可視化される）
		allEnemies.push_back(enemy);
	}
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