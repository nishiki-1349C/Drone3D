#pragma once
#include <cmath>
//ƒxƒNƒgƒ‹’è‹`‚ÆŒvZ
struct Vector {
	float x, y, z;


	Vector() : x(0), y(0), z(0) {}
	//OŸŒ³
	Vector(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	//“ñŸŒ³
	Vector(float X, float Y) : x(X), y(Y), z(0) {}

	//‘«‚µZ
	Vector operator+(const Vector& other)const;
	//ˆø‚«Z
	Vector operator-(const Vector& other)const;
	//Š|‚¯Z
	Vector operator*(float scalar)const;
	//Š„‚èZ
	Vector operator/(float scalar)const;

	Vector& operator+=(const Vector& other);
	Vector& operator-=(const Vector& other);
	Vector& operator*=(float scalar);
	Vector& operator/=(float scalar);

	//â‘Î’l
	float magnitude(const Vector& a, const Vector& b);
	//“àÏ
	float dot(const Vector& a, const Vector& b);
	//ŠOÏ
	Vector cross(const Vector& a, const Vector& b);
};
