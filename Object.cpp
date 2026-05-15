#include "Object.h"
#include "CalcVertices.h"
#include "ObjectStatus.h"
#include "Renderer3D.h"
#include "Sequence.h"
#include "VisTypes.h"   // Vertex 構造体
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <limits>
using namespace glm;
using namespace std;

// 静的メンバ変数の定義
std::vector<Object*> Object::allObjects;
std::vector<Object*> Object::allDynamics;
std::vector<Object*> Object::allEnvironments;

// コンストラクタ
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

// デストラクタ
Object::~Object() {
	if ( renderer ) {              //  nullptr チェック：例外処理が起こる
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
// デストラクタ用ヘルパー
void Object::eraseObjectFrom(std::vector<Object*>& objs, Object* obj) {

	objs.erase(
		remove(objs.begin(), objs.end(), obj),
		objs.end()
	);
}

void Object::update() {
	updateAABBox();
	// オブジェクトごとの処理をここに追加
}

// AABBoxの初期化
void Object::initAABBox() {
	AABBoxMax = currentPos + size * 0.5f;
	AABBoxMin = currentPos - size * 0.5f;
}

// 衝突頂点の更新
void Object::calcColVertices() {
	if ( objType == ObjectType::dynamic ) {
		glm::mat4 model = this->getModelMatrix();
		for ( glm::vec3& v : colVertices ) {
			v = glm::vec3(model * glm::vec4(v, 1.0f));
		}
	}
}

// AABBoxの更新
void Object::updateAABBox() {
	//移動に伴ってAABBoxの頂点を更新
	if ( objType == ObjectType::environment ) return;
	AABBoxMin = currentPos - size * 0.5f;
	AABBoxMax = currentPos + size * 0.5f;
}

// AABBox同士の衝突判定
/*bool Object::checkCollision(Object* other) {
	if ( other == this ) { return false; }
	// 自分自身との衝突は無視
	// 全軸で重なっていれば衝突
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


// 衝突相手otherと自身の衝突処理
void Object::onCollision(Object* other) {}

void Object::resolveOverlap(Object* other) {
	if ( objType == ObjectType::dynamic || objType == ObjectType::other ) {
		//透過しない処理
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

// モデル行列の計算
glm::mat4 Object::getModelMatrix() const {
	glm::mat4 m = glm::mat4(1.0f);
	m = glm::translate(m, currentPos);
	m = glm::rotate(m, rotation.y, glm::vec3(0, 1, 0));
	m = glm::rotate(m, rotation.x, glm::vec3(1, 0, 0));
	m = glm::rotate(m, rotation.z, glm::vec3(0, 0, 1));
	m = glm::scale(m, scale);
	return m;
}

