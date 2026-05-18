// Player.h
#pragma once
class Player {
public:
	Player(int hp, int score, int attack);

	void init();

	static Player* player; // シングルトン用ポインタ

	int hp,
		score,
		attack;
private:
};