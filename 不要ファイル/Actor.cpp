/*
#include "Actor.h"
#include "Object.h"

// static メンバ定義
std::vector<Actor*> Actor::allActors;

// コンストラクタ
Actor::Actor(ShapeType type,
	glm::vec3 color,
	glm::vec3& size,
	glm::vec3& pos)
	: Object(type, color, size, pos) {
	allActors.push_back(this);
}

Actor::Actor(ShapeType type,
	std::string name,
	glm::vec3 color,
	glm::vec3& size,
	glm::vec3& pos)
	: Object(type, name, color, size, pos) {
	allActors.push_back(this);
}



Actor::~Actor() {
	auto it = std::find(allActors.begin(), allActors.end(), this);
	if ( it != allActors.end() ) {
		allActors.erase(it);
	}
}

// ワールド座標におけるAABB更新
void Actor::updateAABBox() {
	AABBoxMin = glm::vec3(std::numeric_limits<float>::max());
	AABBoxMax = glm::vec3(std::numeric_limits<float>::lowest());
	if ( colVertices.empty() ) return;

	for ( const auto& ver : colVertices ) {
		glm::vec3 worldPos = ver + currentPos;
		AABBoxMin.x = std::min(AABBoxMin.x, worldPos.x);
		AABBoxMin.y = std::min(AABBoxMin.y, worldPos.y);
		AABBoxMin.z = std::min(AABBoxMin.z, worldPos.z);
		AABBoxMax.x = std::max(AABBoxMax.x, worldPos.x);
		AABBoxMax.y = std::max(AABBoxMax.y, worldPos.y);
		AABBoxMax.z = std::max(AABBoxMax.z, worldPos.z);
	}
}

void Actor::updateAllAABBox() {
	for ( Actor* actor : allActors ) {
		if ( actor ) actor->updateAABBox();
	}
}

bool Actor::isAABBCollide(const Object& a, const Object& b) {
	return
		a.AABBoxMin.x <= b.AABBoxMax.x && a.AABBoxMax.x >= b.AABBoxMin.x &&
		a.AABBoxMin.y <= b.AABBoxMax.y && a.AABBoxMax.y >= b.AABBoxMin.y &&
		a.AABBoxMin.z <= b.AABBoxMax.z && a.AABBoxMax.z >= b.AABBoxMin.z;
}

void Actor::calcAABBCollision(Actor& self, const Object& other) {
	if ( !isAABBCollide(self, other) ) return;

	float overlapX = std::min(other.AABBoxMax.x - self.AABBoxMin.x, self.AABBoxMax.x - other.AABBoxMin.x);
	float overlapY = std::min(other.AABBoxMax.y - self.AABBoxMin.y, self.AABBoxMax.y - other.AABBoxMin.y);
	float overlapZ = std::min(other.AABBoxMax.z - self.AABBoxMin.z, self.AABBoxMax.z - other.AABBoxMin.z);

	if ( overlapX <= overlapY && overlapX <= overlapZ ) {
		self.currentPos.x += (self.currentPos.x < other.currentPos.x ? -overlapX : overlapX);
	}
	else if ( overlapY <= overlapZ ) {
		self.currentPos.y += (self.currentPos.y < other.currentPos.y ? -overlapY : overlapY);
	}
	else {
		self.currentPos.z += (self.currentPos.z < other.currentPos.z ? -overlapZ : overlapZ);
	}

	self.updateAABBox(); // 衝突後に再計算
}

bool  Actor::calcAllAABBCollision() {
	bool collided = false;
	Actor& self = *this;
	for ( Object* obj : Object::allObjects ) {
		if ( !obj || obj == this ) continue;
		if ( isAABBCollide(self, *obj) ) {
			calcAABBCollision(self, *obj);

			collided = true;
		}
	}
	return collided;
}*/