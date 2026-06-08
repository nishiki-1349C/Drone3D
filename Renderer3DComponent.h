#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "VisTypes.h"   // Vertex
#include "Object.h"
// 色定数（Renderer3D.cpp で定義）
extern glm::vec3 white;
extern glm::vec3 black;
extern glm::vec3 red;
extern glm::vec3 green;
extern glm::vec3 blue;
extern glm::vec3 yellow;
class Renderer3DComponent {
public:
	// メインシェーダーとシャドウシェーダーをコンパイル・リンクする
	static void initShaders(GLuint& outShaderProgram, GLuint& outShadowShader);

	// obj->visVertices の頂点データを VAO/VBO に転送する（init時に1回だけ呼ぶ）
	static void setupMainMesh(GLuint& VAO, GLuint& VBO, Object* obj);

	// 影描画用の空の VAO/VBO を生成する（毎フレーム動的に更新される）
	static void setupShadowMesh(GLuint& shadowVAO, GLuint& shadowVBO);

	// 行列をシェーダーに渡して描画する
	static void drawMain(GLuint shaderProgram, GLuint VAO,
		const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj,
		size_t vertexCount);

	// 影の頂点データを VBO に更新してから影を描画する
	static void drawShadow(GLuint shadowShader, GLuint shadowVAO, GLuint shadowVBO,
		const std::vector<glm::vec3>& shadowVerts,
		const glm::mat4& view, const glm::mat4& proj);

	// VAO/VBO を GPU から削除してIDを0にリセットする
	static void cleanupMesh(GLuint& VAO, GLuint& VBO, GLuint& shadowVAO, GLuint& shadowVBO);

private:
	// メイン描画用シェーダー（頂点・フラグメント）をコンパイル・リンクする
	static void compileMainShader(GLuint& outProgram);

	// 影描画用シェーダー（頂点・フラグメント）をコンパイル・リンクする
	static void compileShadowShader(GLuint& outProgram);

	// 影の頂点データを毎フレーム VBO に再転送する
	static void updateShadowBuffer(GLuint shadowVBO, const std::vector<glm::vec3>& shadowVerts);

	// メインシェーダーと VAO をバインドする
	static void bindMain(GLuint shaderProgram, GLuint VAO);

	// シャドウシェーダーと VAO をバインドする
	static void bindShadow(GLuint shadowShader, GLuint shadowVAO);
};