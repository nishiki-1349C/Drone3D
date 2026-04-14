#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

class CalcVector {
public:

	// 入射ベクトルと法線ベクトルから反射ベクトルを計算
	static glm::vec3 reflection(glm::vec3 vectorIn, glm::vec3 norm);

	//引き算
	//static glm::vec3 toObject(glm::vec3 start, glm::vec3 end);

	//光源から出てオブジェクトに反射した光がカメラに向かうかの判定
	static float  reflectToCamera(glm::vec3 lightPos, glm::vec3 objPos, glm::vec3 norm, glm::vec3 camPos);


};

// ベクトルab ベクトルacを用いた外積による法線計算
inline static glm::vec3 calcFaceNormal(
	const glm::vec3& a,
	const glm::vec3& b,
	const glm::vec3& c
) {
	return normalize(cross(b - a, c - a));
}
