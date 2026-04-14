#pragma once
#include "Object.h"


class Bullet :public Object {
public:
	//コンストラクタ
	Bullet();
	//デストラクタ
	~Bullet() override final;

	//弾が飛んでいく処理
	void update() override final;
	//衝突処理
	void onCollision(Object* other) override final;

	bool isDead = false;

private:
	//Camera* cam;
	glm::vec3 forward;
	float speed;
	float lifeTime; // 弾の寿命（秒）
};