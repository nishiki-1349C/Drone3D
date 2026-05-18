#pragma once
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <vector>

class Object;

class CalcVertices {
public:
	// colVertices 用：形状の頂点座標リストを返す
	static std::vector<glm::vec3> calcVertices_box(glm::vec3 size);
	static std::vector<glm::vec3> calcVertices_sphere(float radius, int verNum);

	// visVertices 用：Object の visVertices に 
	// （位置・法線・色）を追加する
	static void addVisVertices_box(Object* obj);
	static void addVisVertices_sphere(Object* obj, int verNum);
	//static void addVisVertices_cylinder(Object* obj, int verNum);

	static const int verNum_Sphere = 64;

private:
	static constexpr float PI = glm::pi<float>();

	// 三角形 1 枚を法線付きで visVertices に追加する
	static void pushTriangle(Object* obj,
		const glm::vec3& v0,
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& color);
	// 四角形一枚を三角形二枚で表現する
	static void pushQuad(Object* obj,
		const glm::vec3& v0,
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const glm::vec3& color);
};