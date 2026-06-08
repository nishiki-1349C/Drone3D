#pragma once

enum class ObjType {
	None = 0,

	MainDrone,
	Enemy,

	Wall,
	Floor,
	Ceiling,

	//Item
	CheckPoint,
	Goal,
};