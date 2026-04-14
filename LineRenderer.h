#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Renderer3D.h"

struct LineVertex {
	glm::vec3 position;
	glm::vec3 color;
};

class LineRenderer {
public:
	LineRenderer(Camera* cam);
	~LineRenderer();
	void init();
	void drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);

private:
	Camera* cam;
	void drawLineInternal(const LineVertex* verts, int count);

	GLuint VAO = 0;
	GLuint VBO = 0;
	static GLuint lineShader;
	static bool shaderInitialized;

	static void compileLineShader();
};
