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



Camera::Camera()
//:Object(ShapeType::box, ObjectTag::cam, ObjectType::cam, color, vec3(1), Camera::initCamPos)
	:size(vec3(1)),
	camPos(Camera::initPos)
	, target(&MainDrone::mainDrone->currentPos)
	, forward(normalize(*target - camPos))
	, viewWidth(GLFWWrap::winWidth)
	, viewHeight(GLFWWrap::winHeight)
	, viewAngle(45.0f)
	, camPosSpeed(30.0f)
	, yaw(0.0f)
	, pitch(0.0f)
	, camLength(camLengthMax) {
	// シングルトン登録
	cam = this;

	if ( target != nullptr ) { forward = normalize(*target - camPos); }
	else { forward = vec3(0, 0, 1); }

	view = lookAtLH(camPos, *target, vec3(0, 1, 0));

	// プロジェクション行列（固定）
	// 第三引数：近クリップ面距離　第四引数：遠クリップ面距離 = 描写遠近の限界範囲
	proj = perspectiveLH(radians(viewAngle),
		float(viewWidth) / float(viewHeight),
		camLength * 0.1f,
		1000.0f);

	// コライダー頂点の設定
	//colVertices = CalcVertices::calcVertices_box(size);

	//camLengthの最小値
	auto size = MainDrone::mainDrone->size;
	camLengthMin = 1.2f * std::max({size.x, size.y, size.z});

	updateAABBox();

	//カメラがallObjectsに含まれるかどうか
	//for ( Object* obj : Object::allObjects ) { if ( obj->getObjTag() == ObjectTag::cam )cout << "cam inluded in allObjects"; }
	//for ( Object* obj : Object::allDynamics ) { if ( obj->getObjTag() == ObjectTag::cam )cout << "cam inluded in alldynamics"; }
}

void Camera::update() {
	forward = normalize(*target + targetOffset - camPos);
	updateYawPitch();
	moveCamPos();
	resolveCamCollision();
	moveCamPos();
	updateAABBox();
	calcViewMatrix();
}

void Camera::updateAABBox() {
	AABBmax = camPos + size * 0.5f;
	AABBmin = camPos - size * 0.5f;
}

void Camera::updateYawPitch() {
	/*yaw += GLFWWrap::deltaMouseXPos * camPosSpeed;
	pitch += GLFWWrap::deltaMouseYPos * camPosSpeed;*/
	if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_RIGHT) == GLFW_PRESS ) { yaw += camPosSpeed * 2 * TimeMgr::getFixedDelta(); }
	if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_LEFT) == GLFW_PRESS ) { yaw -= camPosSpeed * 2 * TimeMgr::getFixedDelta(); }
	if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_UP) == GLFW_PRESS ) { pitch += camPosSpeed * TimeMgr::getFixedDelta(); }
	if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_DOWN) == GLFW_PRESS ) { pitch -= camPosSpeed * TimeMgr::getFixedDelta(); }

	if ( yaw >= 360 ) { yaw -= 360; }
	else if ( yaw < 0 ) { yaw += 360; }
	pitch = std::clamp(pitch, -pitchRange, pitchRange);
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
	camPos = MainDrone::mainDrone->currentPos - temp;
}

//カメラの衝突 // 非衝突が続いたフレーム数
static int noHitCount_camera = 0;
void Camera::resolveCamCollision() {

	bool isColliding = false;

	// 衝突判定
	for ( Object* env : Object::allEnvironments ) {
		if ( Camera::checkCollision(env) ) {
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

bool Camera::checkCollision(Object* other) {
	if ( AABBmax.x > other->getAABBoxMin().x && AABBmin.x <= other->getAABBoxMax().x &&
		AABBmax.y > other->getAABBoxMin().y && AABBmin.y < other->getAABBoxMax().y &&
		AABBmax.z > other->getAABBoxMin().z && AABBmin.z < other->getAABBoxMax().z ) {
		return true;
	}
	return false;
}


/*void Camera::onCollision(Object* other) {
	cout << "camera collision checked" << endl;
}*/

void Camera::calcViewMatrix() { view = lookAtLH(camPos, MainDrone::mainDrone->currentPos + targetOffset, vec3(0, 1, 0)); }