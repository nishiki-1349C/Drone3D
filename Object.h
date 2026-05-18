#pragma once
#include "ObjectStatus.h"
#include "VisTypes.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <limits>

class Renderer3D;

class Object {
public:
	// コンストラクタ・デストラクタ
	//Object() = default;
	Object(ShapeType shape, ObjectTag tag, ObjectType objType, glm::vec3 color, glm::vec3 size, glm::vec3 pos);
	virtual ~Object();

	static void eraseObjectFrom(std::vector<Object*>& objs, Object* obj);

	virtual void initAABBox();							// AABBoxの初期化
	virtual void update();								// 毎フレームの処理全般
	virtual void calcColVertices();
	//virtual bool checkCollision(Object* other);			// AABBox同士の衝突判定
	virtual void onCollision(Object* other);			//衝突時の処理 
	virtual void resolveOverlap(Object* other) final;	//衝突時の重なり解消
	virtual glm::mat4 getModelMatrix() const;			// モデル行列の計算

	// ゲッター
	virtual const glm::vec3& getAABBoxMin() const { return AABBoxMin; }
	virtual const glm::vec3& getAABBoxMax() const { return AABBoxMax; }
	virtual const ShapeType& getShapeType() const { return shapeType; }
	virtual const ObjectType& getObjType() const { return objType; }
	virtual const ObjectTag& getObjTag() const { return objTag; }

	// 描画用頂点
	std::vector<Vertex> visVertices;

	glm::vec3   color = glm::vec3(1);
	glm::vec3   size = glm::vec3(0);
	glm::vec3   currentPos = glm::vec3(0);

	static std::vector<Object*> allObjects, allDynamics, allEnvironments;

protected:
	void updateAABBox();// AABBoxの更新

	Renderer3D* renderer = nullptr;

	glm::vec4   modelMat;
	glm::vec3   scale = glm::vec3(1);
	glm::vec3   rotation = glm::vec3(0);
	ShapeType   shapeType;
	ObjectTag	objTag;
	ObjectType   objType;

	//衝突判定用
	std::vector<glm::vec3> colVertices;
	glm::vec3   AABBoxMin = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3   AABBoxMax = glm::vec3(std::numeric_limits<float>::lowest());	//minは正の値の最小値、lowestは負の値の最小値なので注意

	bool isTriggered = false;
};