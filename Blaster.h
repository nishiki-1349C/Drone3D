#pragma once
#include "Bullet.h"
#include "GLFWWrap.h"
#include <vector>

//e’e‚Ì¶¬‹y‚Ñíœˆ—
class Blaster {
public:
	static void init();
	static void shot();
	static void eraseBullet(Bullet* bullet);

	static glm::vec3 getBulletSpawnPos() { return bulletSpawnPos; }
private:
	static glm::vec3 bulletSpawnPos;

	static float shotTimer;
	static float shotInterval;

	static std::vector<Bullet*> bullets;

};