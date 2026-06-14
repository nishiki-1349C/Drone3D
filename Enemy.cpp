#include "Enemy.h"
#include "EnemyData.h"
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
	attack(getEnemyDataTable().at(type).attack) {}

//Enemy::~Enemy() {}