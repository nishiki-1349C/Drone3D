#include "Camera.h"
#include "CalcVertices.h"
#include "GLFWWrap.h"
#include "MainDrone.h"
#include "Sequence.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
using namespace glm;
using namespace std;

// 定数定義
const float Camera::pitchRange = 65.0f;
const vec3 Camera::initCamPos = MainDrone::initPos + vec3(0, 10, 0);


// 静的メンバの実体定義
Camera* Camera::cam = nullptr;
float Camera::camLengthMax = 15.0f;
float Camera::camLengthMin;
float Camera::camZoomSpeed = 0.1f;
//vec3 color = vec3(0);

Camera::Camera()
	:Object(ShapeType::box, ObjectTag::cam, ObjectType::cam, color, vec3(1), Camera::initCamPos)
	, target(&MainDrone::mainDrone->currentPos)
	, forward(normalize(*target - currentPos))
	, viewWidth(GLFWWrap::winWidth)
	, viewHeight(GLFWWrap::winHeight)
	, viewAngle(45.0f)
	, camPosSpeed(0.011f)
	, yaw(0.0f)
	, pitch(0.0f)
	, camLength(camLengthMax) {
	// シングルトン登録
	cam = this;

	if ( target != nullptr ) { forward = normalize(*target - currentPos); }
	else { forward = vec3(0, 0, 1); }

	view = lookAtLH(currentPos, *target, vec3(0, 1, 0));

	// プロジェクション行列（固定）
	// 第三引数：近クリップ面距離　第四引数：遠クリップ面距離 = 描写遠近の限界範囲
	proj = perspectiveLH(radians(viewAngle),
		float(viewWidth) / float(viewHeight),
		camLength * 0.1f,
		1000.0f);

	// コライダー頂点の設定
	colVertices = CalcVertices::calcVertices_box(size);

	//camLengthの最小値
	auto size = MainDrone::mainDrone->size;
	camLengthMin = 1.2f * std::max({size.x, size.y, size.z});

	updateAABBox();

	//カメラがallObjectsに含まれるかどうか
	for ( Object* obj : Object::allObjects ) { if ( obj->getObjTag() == ObjectTag::cam )cout << "cam inluded in allObjects"; }
	for ( Object* obj : Object::allDynamics ) { if ( obj->getObjTag() == ObjectTag::cam )cout << "cam inluded in alldynamics"; }

}

void Camera::update() {
	Object::update();

	forward = normalize(*target - currentPos);
	updateYawPitch();
	resolveCamCollision();
	moveCamPos();
	calcViewMatrix();


}

void Camera::updateYawPitch() {
	yaw += GLFWWrap::deltaMouseXPos * camPosSpeed;
	pitch += GLFWWrap::deltaMouseYPos * camPosSpeed;

	if ( yaw >= 360 ) { yaw -= 360; }
	else if ( yaw < 0 ) { yaw += 360; }
	pitch = clamp(pitch, -pitchRange, -5.0f);
	cout << pitch << endl;
}

void Camera::moveCamPos() {
	// カメラの位置 極座標（r,θ,φ）＝（camLength,yaw, pitch）からデカルト座標への変換
	// ただしcameraのポジションはその反対側
	float yawRad = radians(yaw);
	float pitchRad = radians(pitch);

	float x = sin(yawRad) * cos(pitchRad);
	float y = sin(pitchRad);
	float z = cos(yawRad) * cos(pitchRad);
	vec3 temp = camLength * vec3(x, y, z);
	currentPos = MainDrone::mainDrone->currentPos - temp;
}

//カメラの衝突 // 非衝突が続いたフレーム数
static int noHitCount_camera = 0;
void Camera::resolveCamCollision() {

	bool isColliding = false;

	// 衝突判定
	for ( Object* env : Object::allEnvironments ) {
		if ( Sequence::checkCollision(this, env) ) {
			isColliding = true;
			break;
		}
	}

	// 衝突している → 近づける
	if ( isColliding ) {
		noHitCount_camera = 0; // リセット
		camLength -= camZoomSpeed;
		if ( camLength < camLengthMin ) camLength = camLengthMin;
		return;
	}

	// 衝突していない → カウントを増やす
	noHitCount_camera++;

	// 10フレーム連続で非衝突なら戻す
	if ( noHitCount_camera > 10 && camLength < camLengthMax ) {
		camLength += camZoomSpeed;
		if ( camLength > camLengthMax ) camLength = camLengthMax;
	}
}


/*void Camera::onCollision(Object* other) {
	cout << "camera collision checked" << endl;
}*/

void Camera::calcViewMatrix() { view = lookAtLH(currentPos, MainDrone::mainDrone->currentPos, vec3(0, 1, 0)); }