#pragma once
#include "Object.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Camera {
public:
	Camera();
	~Camera() = default;


	void update();
	void Reset();							//カメラを初期の向き・距離・位置へ戻す
	static inline Camera* cam = nullptr;

	// 行列とベクトル
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3* target;						//カメラが向く方向
	glm::vec3 targetOffset = glm::vec3(0, 3, 0);
	glm::vec3 forward;						//カメラの視点方向 target- camPos の正規
	glm::vec3 camPos;

	// ゲッター
	const float getYaw() const { return yaw; }
	const float getPitch() const { return pitch; }

private:
	void updateAABBox();
	void resolveCamCollision();
	void updateYawPitch();							//マウスの動きから方向を示す変数を更新
	void moveCamPos();
	void calcViewMatrix();

	bool checkCollision(Object* other);

	glm::vec3 size;
	glm::vec3 AABBmax, AABBmin;
	int viewWidth, viewHeight;						//ビューポートの幅と高さ
	float viewAngle;								//視野角
	float camPosSpeed;								//視点移動速度
	float yaw, pitch;								//yaw は←→、pitch は↑↓の回転角
	float camLength;								//カメラとターゲットの距離

	static inline float camLengthMax = 15.0f;
	static inline float camLengthMin = 1.0f;
	static inline float camZoomSpeed = 0.1f;
	static inline float pitchRange = 65.0f;	//カメラの上下回転の制限角度
	static inline glm::vec3 initPos = glm::vec3(0, 50, 0);

};