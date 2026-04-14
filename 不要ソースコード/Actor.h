/*#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include "Object.h"
using namespace std;

// Actor クラス 影を落とすオブジェクト
class Object;
class Actor : public Object {
public:
	// 全 Actor 一覧
	static std::vector<Actor*> allActors;

	// コンストラクタ
	//基本
	Actor(ShapeType type,
		glm::vec3 color,
		glm::vec3& size,
		glm::vec3& pos);
	//名前あり
	Actor(ShapeType type,
		std::string name,
		glm::vec3 color,
		glm::vec3& size,
		glm::vec3& pos);

	~Actor();

	// AABBを使用した接触判定
	void updateAABBox();
	static void updateAllAABBox();

	bool isAABBCollide(const Object& a, const Object& b);
	void calcAABBCollision(Actor& self, const Object& other);
	bool calcAllAABBCollision();
};
*/