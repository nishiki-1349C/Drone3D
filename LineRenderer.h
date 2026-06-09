#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include <vector>

// 1本の線分データ（ワールド座標 or NDC座標）
struct LineVertex {
	glm::vec3 start, end;
	glm::vec3 color;
	float width;
	bool is3D = false; // trueならdraw()時にworldToNDCで変換
};

class LineRenderer {
public:
	LineRenderer();
	~LineRenderer();
	void EraseLR();

	// 2D（NDC座標で直接登録）
	void addLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec3& color, float width);
	// 3D（ワールド座標で登録、draw()時に変換）
	void addLine3D(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color, float width);
	// lineVerticesを全て描画してクリア
	void draw();

	static void init();
	static void drawAllLineRenderers();

protected:
	glm::vec3 worldToNDC(const glm::vec3& worldPos, Camera* cam);

	GLuint VAO = 0, VBO = 0;
	std::vector<LineVertex> lineVertices;
	bool depthTest = true; // falseにすると常に前面に描画

	static std::vector<LineRenderer*> allLineRenderers;
	static GLuint lineShader;
	static Camera* cam;
	static bool shaderInitialized;
	static void compileLineShader();
};
