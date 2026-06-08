#include "CalcVector.h"
using namespace glm;

// 反射ベクトルの計算
glm::vec3 CalcVector::reflection(glm::vec3 vectorIn, glm::vec3 norm) {
	glm::vec3 refrect, n;
	// 法線ベクトルを正規化
	n = normalize(norm);

	refrect = vectorIn - 2.0f * glm::dot(vectorIn, n) * n;
	return refrect;
}

float CalcVector::reflectToCamera(glm::vec3 lightPos, glm::vec3 objPos, glm::vec3 norm, glm::vec3 camPos) {
	lightPos = normalize(lightPos);
	norm = normalize(norm);

	return dot(CalcVector::reflection(lightPos, norm) + objPos, objPos - camPos);
};

// オブジェクト方向 つまり引き算
//不要な気がする
/*vec3 CalcVector::toObject(vec3 start, vec3 end) {
	return end - start;
}*/

