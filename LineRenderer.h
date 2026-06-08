#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include <vector>

struct LineVertex {
	glm::vec3 position;
	glm::vec3 color;
	float width;
};

class LineRenderer {
public:
	LineRenderer();
	~LineRenderer();
	void init();

	void drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color, float width = 1.0f);
	void drawLine3D(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color, float width, Camera* cam);

	virtual void draw();

	static void drawAllLineRenderers();
	static std::vector<LineRenderer*> allLineRenderers;

private:
	glm::vec3 worldToNDC(const glm::vec3& worldPos, Camera* cam);

	GLuint VAO = 0, VBO = 0;

	static GLuint lineShader;

	static Camera* cam;
	static bool shaderInitialized;
	static void compileLineShader();
};