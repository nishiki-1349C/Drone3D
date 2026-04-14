#pragma once
#include "Object.h"
#include "ObjectStatus.h"
#include "StageConfig.h"
#include <vector>

enum class StageNum {
	defaultStage = 0,
	stage1,
	stage2,
	stage3,
};

// ステージの構成要素（地面・壁・柱など）を表す構造体
struct StageObjData {
	ShapeType shape;
	ObjectTag tag;
	ObjectType objType = ObjectType::environment;
	glm::vec3 color;
	glm::vec3 size;
	glm::vec3 pos;
	StageNum stageNum;
};

class StageObject : public Object {
public:
	StageObject(const StageObjData& data);
	~StageObject() override = default;

	//	void update()override final;

	static void constructStage();

	StageNum stageNum;
	//void onCollision(Object* other) override;
private:
	static std::vector<StageObjData> objData;
	static void setData();
};