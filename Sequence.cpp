#include "Sequence.h"
#include "Blaster.h"
#include "CalcVertices.h"
#include "EnemyMgr.h"
#include "FadeInOut.h"
#include "GLFWWrap.h"
#include "Player.h"
#include "StageObject.h"
#include "Renderer3D.h"
#include <string>
using namespace glm;
using namespace std;

void Sequence::init() {
	//===== �E�B���h�E�����Ə����� =====
	if ( GLFWWrap::createWindow(GLFWWrap::winWidth, GLFWWrap::winHeight, "Drone3D", nullptr, nullptr) != 0 ) {
		std::cout << "�E�B���h�E�����Ɏ��s���܂���\n";
		return;
	}

	//==== �J�[�\���ݒ� ====
	glfwSetCursorPosCallback(GLFWWrap::window, GLFWWrap::senseMouseMove);
	//glfwSetInputMode(GLFWWrap::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
	glfwSetInputMode(GLFWWrap::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
		std::cout << "Failed to initialize GLAD\n";
		return;
	}


	setInstance();			//�J�����E�h���[���E���̑��C���X�^���X����
	EnemyMgr::spawnEnemy(EnemyType::mob, vec3(0, 0, 50));

	initRenderes();			// Object�̕`��p���_���� ������
	GLFWWrap::init();		// �[�x�@�����ݒ�
	TimeMgr::init(60.0f);	// �Œ�FPS�̎w��
	FadeInOut::init();		// �Ó]�̏�����
}

//�e��C���X�^���X����
void Sequence::setInstance() {
	// �e��R���X�g���N�^
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
	// �`��p���_�̐���
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
	// �S�����_���[��GPU������ (���_���M�Ȃ�)
	Renderer3D::initAllRenderers();
}

void Sequence::mainLoop() {}

// dynamic�ɂ��đS�ẴI�u�W�F�N�g�ɑ΂��ďՓ˔�����s��
void Sequence::checkAllCollision() {
	auto& allDnmcs = Object::allDynamics,
		allEnvs = Object::allEnvironments;

	//dynamic�ɑ΂��đS�Ă�Objs�Ƃ̐ڐG����
	for ( size_t i = 0; i < allDnmcs.size(); ++i ) {
		/*for ( size_t j = 0; j < allObjs.size(); ++j ) {
			if ( allDnmcs [i] == allObjs [j] ) { continue; }	//�������g�Ƃ̔���͖���

			/*auto* A = allDnmcs [i];
			auto* B = allObjs [j];

			if ( checkCollision(A, B) ) {	//�ڐG���Ă����
				allDnmcs [i]->onCollision(allObjs [j]);			//dynamic���̏Փˏ��������s
				allObjs [j]->onCollision(allDnmcs [i]);			//Obj���̏Փˏ��������s

			}

		}*/
		//Dynamics�ɑ΂��Ă̏���
		auto A = allDnmcs [i];
		for ( size_t j = 0; j < allDnmcs.size(); ++j ) {
			auto B = allDnmcs [j];
			if ( i == j ) continue;//�������g�Ƃ̐ڐG�͖���
			if ( checkCollision(A, B) ) {	//�ڐG���Ă����
				A->onCollision(B);			//dynamic���̏Փˏ��������s
				B->onCollision(A);			//Obj���̏Փˏ��������s
			}
		}

		for ( size_t j = 0; j < allEnvs.size(); ++j ) {
			auto B = allEnvs [j];
			if ( checkCollision(A, B) ) {	//�ڐG���Ă����
				A->onCollision(B);			//dynamic���̏Փˏ��������s
				B->onCollision(A);			//Obj���̏Փˏ��������s
			}
		}
	}
}

bool Sequence::checkCollision(Object* a, Object* b) {
	if ( a == b ) { return false; }
	// �������g�Ƃ̏Փ˂͖���
	// AABB�ɂ��Փ�
	if ( a->getAABBoxMax().x >= b->getAABBoxMin().x && a->getAABBoxMin().x <= b->getAABBoxMax().x &&
		a->getAABBoxMax().y >= b->getAABBoxMin().y && a->getAABBoxMin().y <= b->getAABBoxMax().y &&
		a->getAABBoxMax().z >= b->getAABBoxMin().z && a->getAABBoxMin().z <= b->getAABBoxMax().z ) {
		return true;
	}
	return false;
}

bool Sequence::checkTouching(Object* a, Object* b) {
	if ( a == b ) { return false; }
	// �������g�Ƃ̏Փ˖���
	if ( a->getAABBoxMax().x > b->getAABBoxMin().x && a->getAABBoxMin().x < b->getAABBoxMax().x &&
		a->getAABBoxMax().y > b->getAABBoxMin().y && a->getAABBoxMin().y < b->getAABBoxMax().y &&
		a->getAABBoxMax().z > b->getAABBoxMin().z && a->getAABBoxMin().z < b->getAABBoxMax().z ) {
		return true;
	}
	return false;
}

void Sequence::update() {
	//�J�����������O���ĕʂŌĂяo��
	//�S�Ă�Object�ɑ΂���update
	for ( Object* obj : Object::allDynamics ) {
		obj->update();
	}
}