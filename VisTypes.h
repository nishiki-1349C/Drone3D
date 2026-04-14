#pragma once
#include <glm/glm.hpp>

// GPU に送る頂点1個分のデータ（位置・法線・色）
struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
};