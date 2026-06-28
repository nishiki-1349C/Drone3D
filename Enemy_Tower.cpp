#include "Enemy_Tower.h"
#include "MainDrone.h"
#include "TimeMgr.h"
#include "Player.h"
#include "CalcVertices.h"
#include "Renderer3D.h"
#include "LineRenderer.h"
using namespace std;
using namespace glm;

Tower::Tower(glm::vec3 pos)
	: Enemy(EnemyType::tower, pos) {

	// 地面と設置位置の調整
	float groundY = 0.0f;
	for ( Object* obj : Object::allEnvironments ) {
		if ( obj->getAABBoxMax().x >= pos.x && obj->getAABBoxMin().x <= pos.x &&
			obj->getAABBoxMax().z >= pos.z && obj->getAABBoxMin().z <= pos.z ) {
			groundY = std::max(groundY, obj->getAABBoxMax().y);
		}
	}
	currentPos.y = groundY;

	addVisVertices_unique();
	renderer->init();

	laserRenderer = new LineRenderer();
}

void Tower::addVisVertices_unique() {
	float half = shaftBaseSize * 0.5f;
	float topHalf = shaftBaseSize * shaftTopRatio * 0.5f;

	// shaft 底面の4頂点
	vec3 b0(half, 0, half);
	vec3 b1(-half, 0, half);
	vec3 b2(-half, 0, -half);
	vec3 b3(half, 0, -half);

	// shaft 上面 ＝ capstone 底面の4頂点
	vec3 t0(topHalf, shaftHeight, topHalf);
	vec3 t1(-topHalf, shaftHeight, topHalf);
	vec3 t2(-topHalf, shaftHeight, -topHalf);
	vec3 t3(topHalf, shaftHeight, -topHalf);

	// capstone 頂点
	vec3 apex(0, shaftHeight + capstoneHeight, 0);

	// shaft 底面
	CalcVertices::pushQuad(this, b0, b3, b2, b1, color);

	// shaft 側面4面
	CalcVertices::pushQuad(this, b0, b1, t1, t0, color);
	CalcVertices::pushQuad(this, b1, b2, t2, t1, color);
	CalcVertices::pushQuad(this, b2, b3, t3, t2, color);
	CalcVertices::pushQuad(this, b3, b0, t0, t3, color);

	// capstone 側面4面
	CalcVertices::pushTriangle(this, t0, t1, apex, color);
	CalcVertices::pushTriangle(this, t1, t2, apex, color);
	CalcVertices::pushTriangle(this, t2, t3, apex, color);
	CalcVertices::pushTriangle(this, t3, t0, apex, color);
}

void Tower::update() {
	if ( !MainDrone::mainDrone ) return;

	// 1秒前の座標を参照するリングバッファ（予測照準システム）
	//dronePosHistory[historyIndex] = MainDrone::mainDrone->currentPos;
	//historyIndex = (historyIndex + 1) % 60;

	//範囲外なら無視idleを維持
	if ( length(droneDir) >= laserRange ) {
		state = idle;
		idleTimer = idleTime;
	}

	// 状態遷移
	switch ( state ) {
		case idle: // 範囲内なら一時間待機
			idleTimer -= TimeMgr::getFixedDelta();
			if ( idleTimer <= 0 ) { state = standby; }
			break;
		case standby:
			updateDroneDir();

			standbyTimer -= TimeMgr::getFixedDelta();

			// レーザー点滅
			blinkTimer += TimeMgr::getFixedDelta();
			if ( blinkTimer >= blinkInterval ) {
				blinkTimer = 0.0f;
				blinkOn = !blinkOn;
			}
			if ( blinkOn ) {
				vec3 apex = currentPos + vec3(0, shaftHeight + capstoneHeight, 0);
				vec3 target = MainDrone::mainDrone->currentPos;
				laserRenderer->clearLines();
				laserRenderer->addLine3D(apex, target, vec3(1, 0, 0), laserWidthStandby);
			}
			else {
				laserRenderer->clearLines();
			}

			if ( standbyTimer <= 0.0f ) {
				standbyTimer = standbyTime;
				blinkTimer = 0.0f;
				blinkOn = false;
				laserRenderer->clearLines();

				// attack開始時の照準をMainDrone方向に初期化
				vec3 apex = currentPos + vec3(0, shaftHeight + capstoneHeight, 0);
				vec3 toTarget = MainDrone::mainDrone->currentPos - apex;
				aimYaw = atan2(toTarget.x, toTarget.z);
				aimPitch = atan2(toTarget.y, length(vec2(toTarget.x, toTarget.z)));

				state = attack;
			}
			break;
		case attack:
			//レーザー追尾であたり判定もあり。
			updateDroneDir();
			fire();

			attackTimer -= TimeMgr::getFixedDelta();
			if ( attackTimer <= 0.0f ) {
				attackTimer = attackTime;
				lastApex = currentPos + vec3(0, shaftHeight + capstoneHeight, 0);
				lastTarget = currentAimTarget;
				state = overheat;
			}
			break;
		case overheat:
		{
			overheatTimer -= TimeMgr::getFixedDelta();
			float alpha = overheatTimer / overheatTime;

			laserRenderer->clearLines();
			laserRenderer->addLine3D(lastApex, lastTarget, vec3(1, 0, 0), 2.0f, alpha);

			if ( overheatTimer <= 0.0f ) {
				overheatTimer = overheatTime;
				idleTimer = idleTime;
				laserRenderer->clearLines();
				state = idle;
			}
			break;
		}
		case dead:
			break;
	}

	/*
	if ( state == idle ) cout << "Tower: idle" << endl;
	else if ( state == standby ) cout << "Tower: standby" << endl;
	else if ( state == attack ) cout << "Tower: attack" << endl;
	else if ( state == overheat ) cout << "Tower: overheat" << endl;
	else if ( state == dead ) cout << "Tower: dead" << endl;
	*/
}

void Tower::fire() {
	if ( !MainDrone::mainDrone || !laserRenderer ) return;

	vec3 apex = currentPos + vec3(0, shaftHeight + capstoneHeight, 0);

	// 1秒前の座標を参照するリングバッファ（予測照準システム）
	//vec3 target = dronePosHistory[historyIndex];

	// apexからMainDroneへの目標角度を計算
	vec3 toTarget = MainDrone::mainDrone->currentPos - apex;
	float targetYaw = atan2(toTarget.x, toTarget.z);
	float targetPitch = atan2(toTarget.y, length(vec2(toTarget.x, toTarget.z)));

	// yawを上限速度でクランプしながら追従
	float dt = TimeMgr::getFixedDelta();
	float maxYawDelta = radians(maxYawSpeed) * dt;
	float yawDiff = targetYaw - aimYaw;
	// 角度差を[-PI, PI]に正規化
	while ( yawDiff > glm::pi<float>() ) yawDiff -= 2.0f * glm::pi<float>();
	while ( yawDiff < -glm::pi<float>() ) yawDiff += 2.0f * glm::pi<float>();
	aimYaw += glm::clamp(yawDiff, -maxYawDelta, maxYawDelta);
	aimPitch = targetPitch;	// 垂直は上限なし

	// 照準方向からレーザー終点を計算
	float dist = length(toTarget);
	vec3 aimDir = vec3(sin(aimYaw) * cos(aimPitch), sin(aimPitch), cos(aimYaw) * cos(aimPitch));
	vec3 target = apex + aimDir * dist;
	currentAimTarget = target;

	laserRenderer->clearLines();
	laserRenderer->addLine3D(apex, target, vec3(1, 0, 0), laserWidthAttack);

	// スラブ法によるレーザーとMainDroneのAABBox交差判定
	vec3 dir = target - apex;
	vec3 boxMin = MainDrone::mainDrone->getAABBoxMin();
	vec3 boxMax = MainDrone::mainDrone->getAABBoxMax();

	float tMin = 0.0f, tMax = 1.0f;
	bool hit = true;

	for ( int i = 0; i < 3; i++ ) {
		float d = dir [i];
		float ori = apex [i];

		if ( abs(d) < 1e-6f ) {
			// 軸に平行な場合、範囲外なら交差なし
			if ( ori < boxMin [i] || ori > boxMax [i] ) { hit = false; break; }
		}
		else {
			float t0 = (boxMin [i] - ori) / d;
			float t1 = (boxMax [i] - ori) / d;
			if ( t0 > t1 ) swap(t0, t1);
			tMin = std::max(tMin, t0);
			tMax = std::min(tMax, t1);
			if ( tMin > tMax ) { hit = false; break; }
		}
	}

	if ( hit ) {
		laserDamageTimer += TimeMgr::getFixedDelta();
		if ( laserDamageTimer >= laserDamageInterval ) {
			laserDamageTimer = 0.0f;
			Player::player->hp -= attack;
		}
	}
	else {
		laserDamageTimer = 0.0f;
	}
}