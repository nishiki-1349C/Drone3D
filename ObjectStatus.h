#pragma once

enum class ObjectTag {
	none = 0,

	//プレイヤーサイド
	mainDrone,

	//敵
	enemy,

	//武器関連
	//apon,
	bullet,

	wall,
	ground,
	ceiling,

	//Item
	checkPoint,
	goal,
};

enum class ShapeType {
	box,
	sphere,
	other,
};

//名前分かりやすく変えたい
enum class ObjectType {
	dynamic,
	environment,
	other
};