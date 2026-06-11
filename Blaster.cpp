#include "Blaster.h"
#include "Camera.h"
#include "MainDrone.h"
using namespace glm;
using namespace std;

void Blaster::init() {}

void Blaster::shot() {

	//タイマーを進めてインターバルを上回ったら発射
	if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_SPACE) == GLFW_PRESS ) {
		shotTimer += TimeMgr::getFixedDelta();
	}
	if ( shotTimer >= shotInterval ) {
		shotTimer -= shotInterval;

		vec3 forward = Camera::cam->forward;
		bulletSpawnPos = MainDrone::mainDrone->currentPos + forward * 3.0f;
		Bullet* bullet = new Bullet();
		bullets.push_back(bullet);
	}

	//cout << bullets.size() << endl;
}

void Blaster::checkDeadBullets() {
	for ( auto it = bullets.begin(); it != bullets.end(); ) {
		if ( (*it)->isDead ) {
			delete* it;
			it = bullets.erase(it);
		}
		else {
			++it;
		}
	}
}/*
void Blaster::eraseBullet(Bullet* bullet) {
	auto it = std::find(bullets.begin(), bullets.end(), bullet);
	if ( it != bullets.end() ) {
		delete* it;
		bullets.erase(it);
	}
}*/
