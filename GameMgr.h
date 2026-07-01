#pragma once

enum class gameState {
	none,
	ready,
	ingame,
	gameOver,
	loading,
};

class GameMgr {
public:
	static void printState();	// デバッグ用
	static void doPhysicalPhase();
	static void doVisualPhase();
	static void resetGame();

	static inline gameState state = gameState::loading;
	static inline bool isGameOver = true;

private:
	static inline bool shouldGameReset = false;
	static inline float loadTime = 1.0f;
	static inline float loadTimer = loadTime;
	static inline bool isLoaded = false;
};