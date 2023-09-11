#pragma once
#include <random>
#include <iostream>
struct VECTOR3
{
	float x;
	float y;
	float z;

	VECTOR3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	float operator* (VECTOR3& _vector3)
	{
		float dx = x - _vector3.x;
		float dy = y - _vector3.y;
		float dz = z - _vector3.z;

		return (dx * _vector3.x) + (dy * _vector3.y) + (dz * _vector3.z);
	}
};

struct VECTOR2_INT
{
	unsigned int x;
	unsigned int z;

	VECTOR2_INT(unsigned int _x, unsigned int _z) : x(_x), z(_z) {}
};

static float Distance(VECTOR3& _position, VECTOR3& _currentPosition)
{
	return _position* _currentPosition;
}

static int Random(int _max, int _min)
{
	std::random_device random;
	std::mt19937 gen(random());
	std::uniform_int_distribution<int> dist(_min, _max);
	return dist(gen);
}