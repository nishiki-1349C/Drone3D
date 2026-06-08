#include "Bullet.h"
#include "Blaster.h"
#include "CalcVertices.h"
#include "Camera.h"
#include "MainDrone.h"

using namespace std;

Bullet::Bullet()
	:Object(ShapeType::sphere, ObjectTag::bullet, ObjectType::dynamic, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f), Blaster::getBulletSpawnPos()) {

	CalcVertices::addVisVertices_sphere(this, 2);
	renderer->init();

	// �e�̏������x��ݒ�i��: �O���Ɍ������đ�����ԁj
	forward = Camera::cam->forward;		// �h���[���̑O���x�N�g�����g�p
	speed = 300.0f;							// ���x�̑傫���𒲐�
	lifeTime = 10.0f;					// �e�̎�����ݒ�

}

Bullet::~Bullet() {}

void Bullet::update() {
	Object::update();

	// �e��O���Ɉړ�������
	currentPos += forward * speed * TimeMgr::getFixedDelta();

	// �e�̎��������炷
	lifeTime -= TimeMgr::getFixedDelta();
	if ( lifeTime <= 0 ) { isDead = true; } // �������s������e���폜
	//	cout << lifeTime << endl;
}

void Bullet::onCollision(Object* other) {
	// �Փ˂����I�u�W�F�N�g���G�ł���΁A���炩�̃_���[�W�������s��
	auto otherTag = other->getObjTag();
	if ( otherTag != ObjectTag::mainDrone && otherTag != ObjectTag::bullet ) { isDead = true; }

}