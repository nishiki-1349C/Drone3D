#include "Sequence.h"
#include "Blaster.h"
#include "CalcVertices.h"
#include "EnemyMgr.h"
#include "FadeInOut.h"
#include "GLFWWrap.h"
#include "LineRenderer.h"
#include "Player.h"
#include "Renderer3D.h"
#include "StageObject.h"
#include <string>
using namespace glm;
using namespace std;

void Sequence::init() {
	//===== ウィンドウ生成 =====
	if ( GLFWWrap::createWindow(GLFWWrap::winWidth, GLFWWrap::winHeight, "Drone3D", nullptr, nullptr) != 0 ) {
		std::cout << "ウィンドウ生成に失敗しました" << endl;
		return;
	}

	//===== マウスカーソル設定 =====
	glfwSetCursorPosCallback(GLFWWrap::window, GLFWWrap::senseMouseMove);
	//glfwSetInputMode(GLFWWrap::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
	glfwSetInputMode(GLFWWrap::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
		std::cout << "Failed to initialize GLAD\n";
		return;
	}


	setInstance();			// インスタンス生成
	EnemyMgr::spawnEnemy(EnemyType::mob, vec3(0, 0, 50));

	initRenderes();			// Objectの頂点情報を計算
	GLFWWrap::init();		// ウィンドウの初期化
	TimeMgr::init(60.0f);	// 固定FPSの設定
	FadeInOut::init();		// フェードイン・フェードアウトの初期化
	LineRenderer::init();	// LineRendererの初期化
}

// インスタンス生成
void Sequence::setInstance() {
	// メインドローンの生成
	/*shape tag act color size pos */
	MainDrone::mainDrone = new MainDrone(ShapeType::box, ObjectTag::mainDrone, ObjectType::dynamic, vec3(0.5f, 0.7f, 1.0f), vec3(1.5f, 1.5f, 1.5f), MainDrone::initPos);
	EnemyMgr::init();
	Camera::cam = new Camera();
	StageObject::constructStage();
	new Player(100, 0, 10);  // �� �ǉ��ihp, score, attack �͓K�X�j

	//Object* sphere = new Object(ShapeType::sphere, ObjectTag::none, ObjectType::environment, vec3(0.0f, 0.2f, 0.7f), vec3(5), vec3(0, 10, 10));
}

//
void Sequence::initRenderes() {
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
	// すべてのオブジェクトのGPUバッファを初期化
	Renderer3D::initAllRenderers();
}

void Sequence::mainLoop() {}

// dynamicオブジェクト同士の衝突判定
void Sequence::checkAllCollision() {
	auto& allDnmcs = Object::allDynamics,
		allEnvs = Object::allEnvironments;

	//dynamicオブジェクト同士の衝突判定
	for ( size_t i = 0; i < allDnmcs.size(); ++i ) {
		/*for ( size_t j = 0; j < allObjs.size(); ++j ) {
			if ( allDnmcs [i] == allObjs [j] ) { continue; }	//同じオブジェクト同士の判定はスキップ

			/*auto* A = allDnmcs [i];
			auto* B = allObjs [j];

			if ( checkCollision(A, B) ) {	//衝突判定
				allDnmcs [i]->onCollision(allObjs [j]);
				allObjs [j]->onCollision(allDnmcs [i]);

			}

		}*/

		auto A = allDnmcs [i];
		for ( size_t j = 0; j < allDnmcs.size(); ++j ) {
			auto B = allDnmcs [j];
			if ( i == j ) continue;//同じオブジェクト同士の判定はスキップ
			if ( checkCollision(A, B) ) {	//衝突判定
				A->onCollision(B);			//dynamicオブジェクト同士の衝突
				B->onCollision(A);			//Obj同士の衝突
			}
		}

		for ( size_t j = 0; j < allEnvs.size(); ++j ) {
			auto B = allEnvs [j];
			if ( checkCollision(A, B) ) {	//衝突判定
				A->onCollision(B);			//dynamicオブジェクト同士の衝突
				B->onCollision(A);			//Obj同士の衝突
			}
		}
	}
}

bool Sequence::checkCollision(Object* a, Object* b) {
	if ( a == b ) { return false; }
	// 同じオブジェクト同士の判定はスキップ
	// AABBの判定
	if ( a->getAABBoxMax().x >= b->getAABBoxMin().x && a->getAABBoxMin().x <= b->getAABBoxMax().x &&
		a->getAABBoxMax().y >= b->getAABBoxMin().y && a->getAABBoxMin().y <= b->getAABBoxMax().y &&
		a->getAABBoxMax().z >= b->getAABBoxMin().z && a->getAABBoxMin().z <= b->getAABBoxMax().z ) {
		return true;
	}
	return false;
}

bool Sequence::checkTouching(Object* a, Object* b) {
	if ( a == b ) { return false; }
	// 同じオブジェクト同士の判定はスキップ
	if ( a->getAABBoxMax().x > b->getAABBoxMin().x && a->getAABBoxMin().x < b->getAABBoxMax().x &&
		a->getAABBoxMax().y > b->getAABBoxMin().y && a->getAABBoxMin().y < b->getAABBoxMax().y &&
		a->getAABBoxMax().z > b->getAABBoxMin().z && a->getAABBoxMin().z < b->getAABBoxMax().z ) {
		return true;
	}
	return false;
}

void Sequence::update() {
	// 各オブジェクトの更新
	// 動的オブジェクトの更新
	for ( Object* obj : Object::allDynamics ) {
		obj->update();
	}
}