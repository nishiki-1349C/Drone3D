/*#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include "Object.h"
// Environment クラス 影を受ける、動的な処理のないオブジェクト
//　不動のオブジェクト
class Environment : public Object {
public:
	// 全 Environment 一覧
	static std::vector<Environment*> allEnvironments;

	Environment(ShapeType type,
		glm::vec3 color,
		glm::vec3& size,
		glm::vec3& pos);
	Environment(ShapeType type,
		std::string name,
		glm::vec3 color,
		glm::vec3& size,
		glm::vec3& pos);

	~Environment();
};*/