#include "Enemy_Mob.h"

Mob::Mob(glm::vec3 pos)
	: Enemy(EnemyType::mob, pos) {}		// コンストラクタ

void Mob::update() {
	Enemy::update();						// hp<=0でisDead=true の処理を継承

	// Mob固有の動き
	;	// 
}

void Mob::onCollision(Object* other) {}