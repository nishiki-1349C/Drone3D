#include "StageObject.h"
#include <iostream>
using namespace std;

// 静的メンバ変数の定義
std::vector<StageObjData> StageObject::objData;


StageObject::StageObject(const StageObjData& data)
	: Object(data.shape, data.tag, data.objType, data.color, data.size, data.pos) {
	stageNum = data.stageNum;
}

void StageObject::constructStage() {
	//オブジェクトの情報をセット
	StageObject::setData();

	for ( const StageObjData& data : StageObject::objData ) {
		new StageObject(data);
	}
}

void StageObject::setData() {
	//Objectのコンストラクタ引数
	/*	ShapeType shape;
	ObjectTag tag;
	ActorType actorType;
	glm::vec3 color;
	glm::vec3 size;
	glm::vec3 pos;
	StageNum stageNum;*/
	StageObject::objData = {
		// 地面
		{ ShapeType::box, ObjectTag::ground, ObjectType::environment,
		glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(200, 1, 200), glm::vec3(0, -0.5f, 0),
		StageNum::defaultStage },

		// 壁
		{ ShapeType::box, ObjectTag::wall, ObjectType::environment,
		glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(200, wallHeight, 1), glm::vec3(0, wallHeight / 2.0f, -100),
		StageNum::defaultStage },

		{ ShapeType::box, ObjectTag::wall, ObjectType::environment,
		glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(200, wallHeight, 1), glm::vec3(0, wallHeight / 2.0f, 100),
		StageNum::defaultStage },

		 { ShapeType::box, ObjectTag::wall, ObjectType::environment,
		glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1, wallHeight, 200), glm::vec3(-100, wallHeight / 2.0f, 0),
		StageNum::defaultStage },

		{ ShapeType::box, ObjectTag::wall, ObjectType::environment,
		glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1, wallHeight, 200), glm::vec3(100, wallHeight / 2.0f, 0),
		StageNum::defaultStage },
	};
}

//void StageObj::onCollision(Object * other) {};

