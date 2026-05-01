#include "Bullet.h"
#include "Blaster.h"
#include "CalcVertices.h"
#include "Camera.h"
#include "MainDrone.h"
#include "Renderer3D.h"
using namespace std;

Bullet::Bullet()
	:Object(ShapeType::sphere, ObjectTag::bullet, ObjectType::dynamic, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f), Blaster::getBulletSpawnPos()) {

	CalcVertices::addVisVertices_sphere(this, CalcVertices::verNum_Sphere);
	renderer->init();

	// 弾の初期速度を設定（例: 前方に向かって速く飛ぶ）
	forward = Camera::cam->forward;		// ドローンの前方ベクトルを使用
	speed = 3.0f;							// 速度の大きさを調整
	lifeTime = 5.0f;					// 弾の寿命を設定

}

Bullet::~Bullet() {}

void Bullet::update() {
	Object::update();

	// 弾を前方に移動させる
	currentPos += forward * speed;

	// 弾の寿命を減らす
	lifeTime -= TimeMgr::getFixedDelta();
	if ( lifeTime <= 0 ) { isDead = true; } // 寿命が尽きたら弾を削除
	//	cout << lifeTime << endl;
}

void Bullet::onCollision(Object* other) {
	// 衝突したオブジェクトが敵であれば、何らかのダメージ処理を行う
	//if ( other->getObjTag() != ObjectTag::mainDrone ) { isDead = true; }
}