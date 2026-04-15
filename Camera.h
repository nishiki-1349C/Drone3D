#pragma once
#include "Object.h"
#include "GLFWWrap.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Camera {
public:
	Camera();
	~Camera() = default;

	void update();

	static Camera* cam;
	// 行列とベクトル
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3* target;//カメラが向く方向
	glm::vec3 forward;//カメラの視点方向 target- camPos の正規
	glm::vec3 camPos;

	//const Camera* getCam() const { return cam; }
	const float getYaw() const { return yaw; }
	const float getPitch() const { return pitch; }

private:
	void updateAABBox();
	void resolveCamCollision();
	//void onCollision(Object* other) override final;
	void updateYawPitch();//マウスの動きから方向を示す変数を更新
	void moveCamPos();
	void calcViewMatrix();

	bool checkCollision(Object* other);

	glm::vec3 size;
	glm::vec3 AABBmax, AABBmin;
	int viewWidth, viewHeight;//ビューポートの幅と高さ
	float viewAngle;//視野角
	float camPosSpeed;//視点移動速度
	float yaw, pitch;//yaw は←→、pitch は↑↓の回転角
	float camLength;//カメラとターゲットの距離
	static float camLengthMax, camLengthMin, camZoomSpeed;
	const static float pitchRange;//カメラの上下回転の制限角度
	const static glm::vec3 initPos;

};