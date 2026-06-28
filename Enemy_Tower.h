#pragma once
#include "Enemy.h"
#include "LineRenderer.h"
#include <array>

class Tower : public Enemy {
public:
	Tower(glm::vec3 pos);

	void update() override final;
	//void onCollision(Object* other) override final;
	void addVisVertices_unique() override final;
	void fire();

private:
	enum State {
		idle,
		standby,
		attack,
		overheat,
		dead
	};

	State state = idle;

	LineRenderer* laserRenderer = nullptr;

	//ビジュアル系
	static inline float shaftBaseSize = 10.0f;		//台座底面の一辺
	static inline float shaftHeight = 15.0f;		//台座の高さ
	static inline float shaftTopRatio = 0.8f;		//台座上部の比率　＝　上面/底面
	static inline float capstoneHeight = 5.0f;		//頂石の高さ
	static inline float capstoneRatio = 0.3f;		//頂石の比率

	static inline float laserRange = 120.0f;		//レーザーの射程距離
	static inline float laserWidthAttack = 5.0f;	//attack時の太さ
	static inline float laserWidthStandby = 2.0f;	//standby時の太さ
	float laserDamageInterval = 1.0f / 5.0f;	// 秒間5回
	float laserDamageTimer = 0.0f;

	//idle
	float idleTime = 1.0f,
		idleTimer = idleTime;

	//standby
	float standbyTime = 3.0f,
		standbyTimer = standbyTime;
	float blinkInterval = 0.5f,
		blinkTimer = 0.0f;
	bool blinkOn = false;

	//attack
	float attackTime = 5.0f,
		attackTimer = attackTime;

	//overheat
	float overheatTime = 2.0f,
		overheatTimer = overheatTime;
	glm::vec3 lastApex = glm::vec3(0);
	glm::vec3 lastTarget = glm::vec3(0);
	glm::vec3 currentAimTarget = glm::vec3(0);	// fire()で計算した最新の照準終点

	// 1秒前の座標を参照するリングバッファ（予測照準システム）
	//std::array<glm::vec3, 60> dronePosHistory = {};
	//int historyIndex = 0;

	// 角速度による照準制御
	static inline float maxYawSpeed = 20.0f,	// 水平回転上限（度/秒）
		aimYaw = 0.0f,						// 現在の水平照準角度（ラジアン）
		aimPitch = 0.0f;						// 現在の垂直照準角度（ラジアン）
};
