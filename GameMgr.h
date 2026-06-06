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
	static void doPhase();
	static void resetGame();

	static gameState state;
	static bool isGameOver;

private:
	static float loadTime, loadTimer;
};