// Player.h
#pragma once
class Player {
public:
	Player(int hp, int score, int attack);

	void init();

	static inline Player* player = nullptr; // シングルトン用ポインタ

	int hp,
		score,
		attack;
	static inline int initAttack = 10;
private:
};