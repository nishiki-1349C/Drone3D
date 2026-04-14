#pragma once
#include <chrono>

class TimeMgr {
public:
	// 初期化（目標の更新頻度を設定）
	static void init(float targetFps);

	// 毎フレームの冒頭で呼ぶ
	static void update();

	// 物理更新のタイミングかチェック
	static bool shouldUpdate();

	// 物理更新を1回分処理したことを記録
	static void consumeFixedStep();

	// ゲッター
	static const float getDeltaTime() { return deltaTime; };
	static const float getFixedDelta() { return fixedDelta; };
	//カプセル化用のFPSゲッター
	inline static  float targetFPS() { return FPS; }
private:
	using clock = std::chrono::steady_clock;
	using time_point = std::chrono::time_point<clock>;

	static float FPS;
	static float fixedDelta;
	static double timer;
	static time_point currentTime;
	static float deltaTime;
};

//外部からはグローバル関数で呼び出し
//繰り返し使用する関数はクラス名を書きたくない
inline float getFPS() { return TimeMgr::targetFPS(); }