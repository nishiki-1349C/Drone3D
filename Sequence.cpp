#include "Sequence.h"
#include "Blaster.h"
#include "CalcVertices.h"
#include "FadeInOut.h"
#include "GLFWWrap.h"
#include "Object.h"
#include "StageObject.h"
#include "Renderer3D.h"
#include <string>
using namespace glm;
using namespace std;

void Sequence::init() {
	//===== ウィンドウ生成と初期化 =====
	if ( GLFWWrap::createWindow(GLFWWrap::winWidth, GLFWWrap::winHeight, "Drone3D", nullptr, nullptr) != 0 ) {
		std::cout << "ウィンドウ生成に失敗しました\n";
		return;
	}

	//==== カーソル設定 ====
	glfwSetCursorPosCallback(GLFWWrap::window, GLFWWrap::senseMouseMove);
	glfwSetInputMode(GLFWWrap::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
		std::cout << "Failed to initialize GLAD\n";
		return;
	}

	setInstance();			//カメラ・ドローン・その他インスタンス生成
	initRenderes();			// Objectの描画用頂点生成 初期化
	GLFWWrap::init();		// 深度　初期設定
	TimeMgr::init(60.0f);	// 固定FPSの指定
	FadeInOut::init();		// 暗転の初期化
}

//各種インスタンス生成
void Sequence::setInstance() {
	// 各種コンストラクタ
	/*shape tag act color size pos */
	MainDrone::mainDrone = new MainDrone(ShapeType::box, ObjectTag::mainDrone, ObjectType::dynamic, vec3(0.5f, 0.7f, 1.0f), vec3(2, 1, 2), MainDrone::initPos);
	Camera::cam = new Camera();
	StageObject::constructStage();

	Object* sphere = new Object(ShapeType::sphere, ObjectTag::none, ObjectType::environment, vec3(0.0f, 0.2f, 0.7f), vec3(5), vec3(0, 10, 10));
}

//
void Sequence::initRenderes() {
	// 描画用頂点の生成
	for ( Object* obj : Object::allObjects ) {
		switch ( obj->getShapeType() ) {
			case ShapeType::box:
				CalcVertices::addVisVertices_box(obj);
				break;
			case ShapeType::sphere:
				CalcVertices::addVisVertices_sphere(obj, CalcVertices::verNum_Sphere);
				break;
			case ShapeType::other:
				cout << "ShapeType is Other";
			default: break;
		}
	}
	// 全レンダラーのGPU初期化 (頂点送信など)
	Renderer3D::initAllRenderers();
}

void Sequence::mainLoop() {}

// dynamicについて全てのオブジェクトに対して衝突判定を行う
void Sequence::checkAllCollision() {
	auto& allDnmcs = Object::allDynamics,
		allEnvs = Object::allEnvironments;

	//dynamicに対して全てのObjsとの接触処理
	for ( size_t i = 0; i < allDnmcs.size(); ++i ) {
		/*for ( size_t j = 0; j < allObjs.size(); ++j ) {
			if ( allDnmcs [i] == allObjs [j] ) { continue; }	//自分自身との判定は無視

			/*auto* A = allDnmcs [i];
			auto* B = allObjs [j];

			if ( checkCollision(A, B) ) {	//接触していれば
				allDnmcs [i]->onCollision(allObjs [j]);			//dynamic側の衝突処理を実行
				allObjs [j]->onCollision(allDnmcs [i]);			//Obj側の衝突処理を実行

			}

		}*/
		//Dynamicsに対しての処理
		auto A = allDnmcs [i];
		for ( size_t j = 0; j < allDnmcs.size(); ++j ) {
			auto B = allDnmcs [j];
			if ( i == j ) continue;//自分自身との接触は無視
			if ( checkCollision(A, B) ) {	//接触していれば
				A->onCollision(B);			//dynamic側の衝突処理を実行
				B->onCollision(A);			//Obj側の衝突処理を実行
			}
		}

		for ( size_t j = 0; j < allEnvs.size(); ++j ) {
			auto B = allEnvs [j];
			if ( i == j ) continue;//自分自身との接触は無視
			if ( checkCollision(A, B) ) {	//接触していれば
				A->onCollision(B);			//dynamic側の衝突処理を実行
				B->onCollision(A);			//Obj側の衝突処理を実行
			}
		}
	}
}

bool Sequence::checkCollision(Object* a, Object* b) {
	if ( a == b ) { return false; }
	// 自分自身との衝突は無視
	// AABBによる衝突
	if ( a->getAABBoxMax().x >= b->getAABBoxMin().x && a->getAABBoxMin().x <= b->getAABBoxMax().x &&
		a->getAABBoxMax().y >= b->getAABBoxMin().y && a->getAABBoxMin().y <= b->getAABBoxMax().y &&
		a->getAABBoxMax().z >= b->getAABBoxMin().z && a->getAABBoxMin().z <= b->getAABBoxMax().z ) {
		return true;
	}
	return false;
}

bool Sequence::checkTouching(Object* a, Object* b) {
	if ( a == b ) { return false; }
	// 自分自身との衝突無視
	if ( a->getAABBoxMax().x > b->getAABBoxMin().x && a->getAABBoxMin().x < b->getAABBoxMax().x &&
		a->getAABBoxMax().y > b->getAABBoxMin().y && a->getAABBoxMin().y < b->getAABBoxMax().y &&
		a->getAABBoxMax().z > b->getAABBoxMin().z && a->getAABBoxMin().z < b->getAABBoxMax().z ) {
		return true;
	}
	return false;
}

void Sequence::update() {
	//カメラだけ除外して別で呼び出し
	//全てのObjectに対するupdate
	for ( Object* obj : Object::allDynamics ) {
		obj->update();
	}
}