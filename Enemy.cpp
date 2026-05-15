#include "Enemy.h"
#include "EnemyData.h"
#include "MainDrone.h"

// 静的メンバ変数の定義
glm::vec3 Enemy::plPos = MainDrone::mainDrone->currentPos;

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
	attack(getEnemyDataTable().at(type).attack),
	speed(getEnemyDataTable().at(type).speed) {}

Enemy::~Enemy() {}

void Enemy::update() {
	if ( hp <= 0 ) isDead = true;
}

void Enemy::onCollision(Object* other) {}