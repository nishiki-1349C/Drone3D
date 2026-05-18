#include "Enemy.h"
#include "EnemyData.h"
#include "MainDrone.h"
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
	state(-1),
	hp(getEnemyDataTable().at(type).hp),
	attack(getEnemyDataTable().at(type).attack) {}

//Enemy::~Enemy() {}



void Enemy::update() {
	enemyTimer += TimeMgr::getFixedDelta();
	if ( hp <= 0 ) isDead = true;
}

void Enemy::onCollision(Object* other) {}