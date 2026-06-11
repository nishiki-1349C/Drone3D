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

	static inline std::vector<Bullet*> bullets;
private:
	static inline glm::vec3 bulletSpawnPos = glm::vec3(0.0f);

	static inline float shotTimer = 0.0f;
	static inline float shotInterval = 0.5f;



};