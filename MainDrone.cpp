#include "MainDrone.h"
#include "Blaster.h"
using namespace glm;
using namespace std;

//静的実体定義
MainDrone* MainDrone::mainDrone = nullptr;
vec3 MainDrone::initPos = vec3(0, 40, 0);
vec3 MainDrone::forward = vec3(0, 0, 1);
vec3 MainDrone::right = vec3(1, 0, 0);

// コンストラクタ
MainDrone::MainDrone(ShapeType shape, ObjectTag obj, ObjectType act, vec3 color, vec3 size, vec3 pos)
//Object::Object(ShapeType shape, ObjectType obj, dynamicType act, vec3 color, vec3 size, vec3 pos)
	:Object(shape, obj, act, color, size, pos),
	lastPos(pos),
	droneVelocity(vec3(0)),
	isOverSpeed(false),
	gravityScale(0.5f) {
	MainDrone::mainDrone = this;
}

//デストラクタ
MainDrone::~MainDrone() {
	if ( mainDrone == this ) { mainDrone = nullptr; }
}

//void MainDrone::calcDroneVelocity() { lastPos = currentPos; }

// 物理更新
void MainDrone::update() {
	if ( !Camera::cam ) return;
	updateAABBox();
	move();
	//calcDroneVelocity();



	if ( currentPos.y >= wallHeight ) { isGameOver = true; }

	lastPos = currentPos;
}

void MainDrone::move() {
	operate();

	//normalizeに零ベクトル渡すとNaNになるため対策
	if ( glm::length(right) < 1e-6f ) right = vec3(1, 0, 0);
	else right = glm::normalize(right);

	// 水平方向
	glm::vec2 velocityXZ = glm::vec2(droneVelocity.x, droneVelocity.z);
	if ( glm::length(velocityXZ) > horizontalSpeedLimit ) { velocityXZ = glm::normalize(velocityXZ) * horizontalSpeedLimit; }
	droneVelocity.x = velocityXZ.x * drag;
	droneVelocity.z = velocityXZ.y * drag;

	// 垂直方向（重力と減衰）
	//gravityScale = 0.15f;
	//droneVelocity.y += gravityScale * gravity.y;
	droneVelocity.y *= 0.97f; // 空気抵抗
	droneVelocity.y = glm::clamp(droneVelocity.y, -fallSpeedLimit, 1.5f * fallSpeedLimit);

	// --- 移動と衝突 ---
	currentPos += droneVelocity;
}


void MainDrone::operate() {
	if ( !Camera::cam ) return;

	//===== 前方向と右方向の計算 =====
	// 前方向（+Z が前）
	forward = normalize(vec3(Camera::cam->forward.x, 0, Camera::cam->forward.z));
	if ( glm::length(forward) < 1e-6f ) forward = vec3(0, 0, 1);
	else forward = glm::normalize(forward);

	// 左手系 right = up × forward
	right = glm::cross(vec3(0, 1, 0), forward);

	// 前後移動
	if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_W) == GLFW_PRESS ) { droneVelocity += forward * acceleration; }
	if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_S) == GLFW_PRESS ) { droneVelocity -= forward * acceleration; }

	// 左右移動
	if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_D) == GLFW_PRESS ) { droneVelocity += right * acceleration; }
	if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_A) == GLFW_PRESS ) { droneVelocity -= right * acceleration; }

	// 上昇
	//if ( glfwGetKey(GLFWWrap::window, GLFW_KEY_SPACE) == GLFW_PRESS ) { droneVelocity += vec3(0, 1.0f, 0) * upAcceleration; }


	if ( glfwGetMouseButton(GLFWWrap::window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ) { droneVelocity.y += upAcceleration; }
	if ( glfwGetMouseButton(GLFWWrap::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ) { droneVelocity.y -= upAcceleration; }
}



void MainDrone::onCollision(Object* other) {
	droneVelocity.y = 0;

	//めり込み押し戻し
	if ( other->getObjTag() != ObjectTag::bullet ) { resolveOverlap(other); }

	//衝突による墜落
	/*if ( other->getdynamicType() == dynamicType::environment ) {
		float impactSpeed = glm::length(droneVelocity);
		if ( impactSpeed > crashSpeedThreshold ) {
			isGameOver = true;
		}
	}*/
}

