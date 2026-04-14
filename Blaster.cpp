#include "Blaster.h"
#include "Camera.h"
#include "MainDrone.h"
using namespace glm;
using namespace std;

//Ã“Iƒƒ“ƒo‚ÌÀ‘Ì’è‹`
vector<Bullet*> Blaster::bullets;
vec3 Blaster::bulletSpawnPos = vec3(0.0f);
float Blaster::shotTimer = 0.0f;
float Blaster::shotInterval = 1.0f; // 1•b‚²‚Æ‚É”­Ë

void Blaster::shot() {
	shotTimer += TimeMgr::getDeltaTime();
	if ( shotTimer >= shotInterval ) {
		shotTimer -= shotInterval;
		vec3 forward = Camera::cam->forward;
		bulletSpawnPos = MainDrone::mainDrone->currentPos + forward * 3.0f;
		Bullet* bullet = new Bullet();
		bullets.push_back(bullet);
	}
}

void Blaster::eraseBullet(Bullet* bullet) {
	auto it = std::find(bullets.begin(), bullets.end(), bullet);
	if ( it != bullets.end() ) {
		delete* it;
		bullets.erase(it);
	}
}