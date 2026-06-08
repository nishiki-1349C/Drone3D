#pragma once
#include "Bullet.h"
#include "GLFWWrap.h"
#include <vector>

//銃弾の生成及び削除処理
class Blaster {
public:
	static void init();
	static void shot();
	static void moveShotTimer(float deltaTime) { shotTimer += deltaTime; }
	static void eraseBullet(Bullet* bullet);
	static void checkDeadBullets();

	static glm::vec3 getBulletSpawnPos() { return bulletSpawnPos; }

	static float getShotTimer() { return shotTimer; }
	static float getShotInterval() { return shotInterval; }

	static std::vector<Bullet*> bullets;
private:
	static glm::vec3 bulletSpawnPos;

	static float shotTimer;
	static float shotInterval;



};