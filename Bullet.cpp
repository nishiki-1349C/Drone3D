#include "Bullet.h"
#include "Blaster.h"
#include "CalcVertices.h"
#include "Camera.h"
#include "MainDrone.h"
#include "Renderer3D.h"

using namespace std;

Bullet::Bullet()
	:Object(ShapeType::sphere, ObjectTag::bullet, ObjectType::dynamic, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f), Blaster::getBulletSpawnPos()) {

	CalcVertices::addVisVertices_sphere(this, 2);
	renderer->init();

	forward = Camera::cam->forward;
	speed = 300.0f;
	lifeTime = 10.0f;

}

Bullet::~Bullet() {}

void Bullet::update() {
	Object::update();

	currentPos += forward * speed * TimeMgr::getFixedDelta();

	lifeTime -= TimeMgr::getFixedDelta();
	if ( lifeTime <= 0 ) { isDead = true; }
	//	cout << lifeTime << endl;
}

void Bullet::onCollision(Object* other) {
	auto otherTag = other->getObjTag();
	if ( otherTag != ObjectTag::mainDrone && otherTag != ObjectTag::bullet ) { isDead = true; }

}