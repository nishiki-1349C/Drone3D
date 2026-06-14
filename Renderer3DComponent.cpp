#include "Renderer3DComponent.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Renderer3DComponent::initShaders(GLuint& outShaderProgram, GLuint& outShadowShader) {
	compileMainShader(outShaderProgram);
	compileShadowShader(outShadowShader);
}

// Vertex（pos/normal/color）リストを VBO に転送し VAO に属性を設定する
void Renderer3DComponent::setupMainMesh(GLuint& VAO, GLuint& VBO, Object* obj) {
	std::vector<Vertex>& vertices = obj->visVertices;

	if ( !VAO ) glGenVertexArrays(1, &VAO);
	if ( !VBO ) glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// location 0: pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);
	// location 1: normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	// location 2: color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Renderer3DComponent::setupShadowMesh(GLuint& shadowVAO, GLuint& shadowVBO) {
	if ( !shadowVAO ) glGenVertexArrays(1, &shadowVAO);
	if ( !shadowVBO ) glGenBuffers(1, &shadowVBO);

	glBindVertexArray(shadowVAO);
	glBindBuffer(GL_ARRAY_BUFFER, shadowVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Renderer3DComponent::drawMain(
	GLuint shaderProgram, GLuint VAO,
	const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj,
	size_t vertexCount) {
	bindMain(shaderProgram, VAO);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModel"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProj"), 1, GL_FALSE, glm::value_ptr(proj));

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexCount);
}

void Renderer3DComponent::drawShadow(
	GLuint shadowShader, GLuint shadowVAO, GLuint shadowVBO,
	const std::vector<glm::vec3>& shadowVerts,
	const glm::mat4& view, const glm::mat4& proj) {
	updateShadowBuffer(shadowVBO, shadowVerts);
	bindShadow(shadowShader, shadowVAO);

	glUniformMatrix4fv(glGetUniformLocation(shadowShader, "uView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shadowShader, "uProj"), 1, GL_FALSE, glm::value_ptr(proj));

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)shadowVerts.size());
}

void Renderer3DComponent::cleanupMesh(GLuint& VAO, GLuint& VBO, GLuint& shadowVAO, GLuint& shadowVBO) {
	if ( VAO )       glDeleteVertexArrays(1, &VAO);
	if ( VBO )       glDeleteBuffers(1, &VBO);
	if ( shadowVAO ) glDeleteVertexArrays(1, &shadowVAO);
	if ( shadowVBO ) glDeleteBuffers(1, &shadowVBO);
	VAO = VBO = shadowVAO = shadowVBO = 0;
}

void Renderer3DComponent::updateShadowBuffer(GLuint shadowVBO, const std::vector<glm::vec3>& shadowVerts) {
	glBindBuffer(GL_ARRAY_BUFFER, shadowVBO);
	glBufferData(GL_ARRAY_BUFFER, shadowVerts.size() * sizeof(glm::vec3), shadowVerts.data(), GL_DYNAMIC_DRAW);
}

void Renderer3DComponent::bindMain(GLuint shaderProgram, GLuint VAO) {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
}

void Renderer3DComponent::bindShadow(GLuint shadowShader, GLuint shadowVAO) {
	glUseProgram(shadowShader);
	glBindVertexArray(shadowVAO);
}

void Renderer3DComponent::compileMainShader(GLuint& outProgram) {
	const char* vertSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec3 aColor;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;

        out vec3 FragPos;
        out vec3 Normal;
        out vec3 Color;

        void main() {
            FragPos     = vec3(uModel * vec4(aPos, 1.0));
            Normal      = mat3(transpose(inverse(uModel))) * aNormal;
            Color       = aColor;
            gl_Position = uProj * uView * vec4(FragPos, 1.0);
        }
    )";

	const char* fragSrc = R"(
        #version 330 core
        in vec3 FragPos;
        in vec3 Normal;
        in vec3 Color;

        out vec4 FragColor;

        uniform vec3  uLightPos;
        uniform vec3  uColor;
        uniform float uAlpha;

        void main() {
            vec3  lightDir   = normalize(uLightPos - FragPos);
            float diff       = max(dot(normalize(Normal), lightDir), 0.0);
            float brightness = 0.2 + diff;
            FragColor = vec4(Color * uColor * brightness, uAlpha);
        }
    )";

	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSrc, NULL);
	glCompileShader(vert);

	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSrc, NULL);
	glCompileShader(frag);

	outProgram = glCreateProgram();
	glAttachShader(outProgram, vert);
	glAttachShader(outProgram, frag);
	glLinkProgram(outProgram);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

void Renderer3DComponent::compileShadowShader(GLuint& outProgram) {
	const char* vertSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        uniform mat4 uView;
        uniform mat4 uProj;

        void main() {
            gl_Position = uProj * uView * vec4(aPos, 1.0);
        }
    )";

	const char* fragSrc = R"(
        #version 330 core
        out vec4 FragColor;

        void main() {
            FragColor = vec4(0.0, 0.0, 0.0, 0.5);
        }
    )";

	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSrc, NULL);
	glCompileShader(vert);

	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSrc, NULL);
	glCompileShader(frag);

	outProgram = glCreateProgram();
	glAttachShader(outProgram, vert);
	glAttachShader(outProgram, frag);
	glLinkProgram(outProgram);

	glDeleteShader(vert);
	glDeleteShader(frag);
}