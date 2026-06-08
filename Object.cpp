#include "Object.h"
#include "CalcVertices.h"
#include "ObjectStatus.h"
#include "Renderer3D.h"
#include "VisTypes.h"   // Vertex�\����
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <limits>
using namespace glm;
using namespace std;

// �ÓI�����o�ϐ��̒�`
std::vector<Object*> Object::allObjects;
std::vector<Object*> Object::allDynamics;
std::vector<Object*> Object::allEnvironments;

// �R���X�g���N�^
Object::Object(ShapeType shape, ObjectTag tag, ObjectType act, glm::vec3 color, glm::vec3 size, glm::vec3 pos)
	: shapeType(shape)
	, objTag(tag)
	, objType(act)
	, color(color)
	, size(size)
	, currentPos(pos) {
	allObjects.push_back(this),
		renderer = nullptr;
	switch ( objType ) {
		case ObjectType::dynamic:       allDynamics.push_back(this); break;
		case ObjectType::environment: allEnvironments.push_back(this); break;
		default: break;
	}

	initAABBox();
	renderer = new Renderer3D(this);
}

// �f�X�g���N�^
Object::~Object() {
	if ( renderer ) {              //  nullptr �`�F�b�N�F��O�������N����
		delete renderer;
		renderer = nullptr;
	}

	eraseObjectFrom(allObjects, this);
	switch ( objType ) {
		case ObjectType::dynamic:     eraseObjectFrom(allDynamics, this);     break;
		case ObjectType::environment: eraseObjectFrom(allEnvironments, this); break;
		default: break;
	}
}
// �f�X�g���N�^�p�w���p�[
void Object::eraseObjectFrom(std::vector<Object*>& objs, Object* obj) {

	objs.erase(
		remove(objs.begin(), objs.end(), obj),
		objs.end()
	);
}

void Object::update() {
	updateAABBox();
	// �I�u�W�F�N�g���Ƃ̏����������ɒǉ�
}

// AABBox�̏�����
void Object::initAABBox() {
	AABBoxMax = currentPos + size * 0.5f;
	AABBoxMin = currentPos - size * 0.5f;
}

// �Փ˒��_�̍X�V
void Object::calcColVertices() {
	if ( objType == ObjectType::dynamic ) {
		glm::mat4 model = this->getModelMatrix();
		for ( glm::vec3& v : colVertices ) {
			v = glm::vec3(model * glm::vec4(v, 1.0f));
		}
	}
}

// AABBox�̍X�V
void Object::updateAABBox() {
	//�ړ��ɔ�����AABBox�̒��_���X�V
	if ( objType == ObjectType::environment ) return;
	AABBoxMin = currentPos - size * 0.5f;
	AABBoxMax = currentPos + size * 0.5f;
}

// AABBox���m�̏Փ˔���
/*bool Object::checkCollision(Object* other) {
	if ( other == this ) { return false; }
	// �������g�Ƃ̏Փ˂͖���
	// �S���ŏd�Ȃ��Ă���ΏՓ�
	if ( AABBoxMax.x >= other->AABBoxMin.x && AABBoxMin.x <= other->AABBoxMax.x &&
		AABBoxMax.y >= other->AABBoxMin.y && AABBoxMin.y <= other->AABBoxMax.y &&
		AABBoxMax.z >= other->AABBoxMin.z && AABBoxMin.z <= other->AABBoxMax.z ) {
		cout << "Collision detected between " << objTag << " and " << other->objTag << endl;
		this->onCollision(other);
		other->onCollision(this);
		return true;
	}
	return false;
}*/


// �Փˑ���other�Ǝ��g�̏Փˏ���
void Object::onCollision(Object* other) {}

void Object::resolveOverlap(Object* other) {
	if ( objType == ObjectType::dynamic || objType == ObjectType::other ) {
		//���߂��Ȃ�����
		float overlapX = std::min(AABBoxMax.x, other->AABBoxMax.x) - std::max(AABBoxMin.x, other->AABBoxMin.x);
		float overlapY = std::min(AABBoxMax.y, other->AABBoxMax.y) - std::max(AABBoxMin.y, other->AABBoxMin.y);
		float overlapZ = std::min(AABBoxMax.z, other->AABBoxMax.z) - std::max(AABBoxMin.z, other->AABBoxMin.z);

		if ( overlapX < overlapY && overlapX < overlapZ ) {
			if ( currentPos.x < other->currentPos.x ) currentPos.x -= overlapX;
			else currentPos.x += overlapX;
		}
		else if ( overlapY < overlapZ ) {
			if ( currentPos.y < other->currentPos.y ) currentPos.y -= overlapY;
			else currentPos.y += overlapY;
		}
		else {
			if ( currentPos.z < other->currentPos.z ) currentPos.z -= overlapZ;
			else currentPos.z += overlapZ;
		}
	}
}

// ���f���s��̌v�Z
glm::mat4 Object::getModelMatrix() const {
	glm::mat4 m = glm::mat4(1.0f);
	m = glm::translate(m, currentPos);
	m = glm::rotate(m, rotation.y, glm::vec3(0, 1, 0));
	m = glm::rotate(m, rotation.x, glm::vec3(1, 0, 0));
	m = glm::rotate(m, rotation.z, glm::vec3(0, 0, 1));
	m = glm::scale(m, scale);
	return m;
}

