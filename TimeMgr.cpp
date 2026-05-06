#include "TimeMgr.h"
#include <algorithm>


float TimeMgr::FPS = 30;
float TimeMgr::fixedDelta = 1.0f / 60.0f;
double TimeMgr::timer = 0.0;
TimeMgr::time_point TimeMgr::currentTime;
float TimeMgr::deltaTime = 0.0f;


void TimeMgr::init(float targetFps) {
	fixedDelta = 1.0f / targetFps;
	currentTime = clock::now();
	timer = 0.0;
}

void TimeMgr::update() {
	auto newTime = clock::now();
	std::chrono::duration<float> frameTime = newTime - currentTime;
	currentTime = newTime;

	// 前フレームからの経過時間（急激な負荷で物理が壊れないよう0.1秒で制限）
	deltaTime = std::min(frameTime.count(), 0.1f);
	timer += deltaTime;
}

bool TimeMgr::shouldUpdate() {
	return timer >= (double)fixedDelta;
}

void TimeMgr::consumeFixedStep() { timer -= (double)fixedDelta; }