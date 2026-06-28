#include "Enemy_Mob.h"
#include "MainDrone.h"
#include "Player.h"
#include "TimeMgr.h"
using namespace std;
using namespace glm;

Mob::Mob(glm::vec3 pos)
	: Enemy(EnemyType::mob, pos),
	baseColor(color) {}		// コンストラクタ

void Mob::update() {
	if ( !MainDrone::mainDrone ) return;
	updateDroneDir();
	//cout << state << endl;

	vec3 target = droneDir;

	if ( hp <= 0 ) state = State::dead;

	// Mob固有の動き
	switch ( state ) {
		case idle:
			idleTimer -= TimeMgr::getFixedDelta();


			if ( idleTimer <= 0 || length(target) < detectionRange ) {
				state = chase;
				idleTimer = idleTime;
			}
			break;
		case chase:
			target.y = 0;	// 水平方向のみに追従
			rotation.y = atan2(target.x, target.z);
			velocity = normalize(target) * chaseSpeed;
			if ( length(target) < attackRange ) {
				state = attack;
				attackTimer = 0;
				isAttacking = false;
				attackDir = vec2(target.x, target.z);
			}
			break;
		case attack:
			attackTimer += TimeMgr::getFixedDelta();

			if ( attackTimer < attackWaitTime ) {
				// 待機中：停止
				velocity.x = 0;
				velocity.z = 0;
			}
			else if ( attackTimer < attackWaitTime + jumpTime ) {
				// ジャンプ開始直後に初速を付与
				if ( !isAttacking ) {
					velocity.y = jumpSpeed;
					velocity.x = normalize(attackDir).x * chaseSpeed * 1.8f;	// ジャンプ中は少し速く移動
					velocity.z = normalize(attackDir).y * chaseSpeed * 1.8f;
					isAttacking = true;
				}
			}
			else if ( attackTimer < attackWaitTime + jumpTime + coolTime ) {
				// クールタイム：攻撃判定解除
				isAttacking = false;
				velocity.x = 0;
				velocity.z = 0;
			}
			else {
				// chase に移行
				attackTimer = 0;
				state = chase;
			}
			break;
		case State::dead:
			deadTimer -= TimeMgr::getFixedDelta();
			alpha = deadTimer / deadTime;
			velocity = vec3(0);
			if ( deadTimer <= 0 ) {
				isDead = true;
			}
			break;
	}

	//位置と速度の更新
	float dt = TimeMgr::getFixedDelta();
	float drag = 0.98f;
	velocity.x *= drag;
	velocity.z *= drag;
	velocity.y -= 20.0f * dt;

	currentPos += velocity * dt;
	Object::update();		// AABBoxの更新など基本的な更新
}

void Mob::onCollision(Object* other) {
	switch ( other->getObjTag() ) {
		case ObjectTag::mainDrone:
			if ( isDead ) { return; }
			if ( isAttacking ) {
				Player::player->hp -= attack;
				MainDrone::mainDrone->resolveOverlap(this);
				isAttacking = false;

				vec3 knockBack = MainDrone::mainDrone->currentPos - currentPos;
				knockBack.y = 0;	// ノックバックの高さを固定
				MainDrone::mainDrone->droneVelocity = normalize(knockBack) * 40.0f;	// ノックバックで少し浮かせる
			}
			else {
				resolveOverlap(other);
			}

			break;
		case ObjectTag::bullet:
			if ( isDead ) { return; }
			hp -= Player::player->attack;
			break;

		case ObjectTag::ground:
			velocity.y = 0;
			resolveOverlap(other);
			break;
		case ObjectTag::wall:
			resolveOverlap(other);
			break;
	}
}