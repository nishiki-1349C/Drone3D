#include "Enemy.h"
#include "EnemyData.h"
#include "MainDrone.h"
#include "TimeMgr.h"
using namespace std;
using namespace glm;

// 静的メンバ変数の定義


Enemy::Enemy(EnemyType type, glm::vec3 pos)
	: Object(
		ShapeType::box,
		ObjectTag::enemy,
		ObjectType::dynamic,
		getEnemyDataTable().at(type).color,  // テーブルから色を取得
		getEnemyDataTable().at(type).size,   // テーブルからサイズを取得
		pos
	),
	hp(getEnemyDataTable().at(type).hp),
	attack(getEnemyDataTable().at(type).attack) {
	// 生成時は即座に初期化
	if ( MainDrone::mainDrone ) droneDir = MainDrone::mainDrone->currentPos - currentPos;
}

//void Enemy::update() {}

//（自分→drone方向）を更新
void Enemy::updateDroneDir() {
	if ( !MainDrone::mainDrone ) return;
	droneDir = MainDrone::mainDrone->currentPos - currentPos;
}

// 1秒あたり perSec 回の頻度で （自分→drone方向）を更新
void Enemy::updateDroneDirPerSec(int perSec) {
	if ( !MainDrone::mainDrone ) return;
	dirTimer += TimeMgr::getFixedDelta();
	if ( dirTimer >= 1.0f / perSec ) {
		dirTimer = 0.0f;
		droneDir = MainDrone::mainDrone->currentPos - currentPos;
	}
}