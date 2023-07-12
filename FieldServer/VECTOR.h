#pragma once

static struct VECTOR3
{
	float x;
	float y;
	float z;

	VECTOR3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

static struct VECTOR2_INT
{
	unsigned int x;
	unsigned int z;

	VECTOR2_INT(unsigned int _x, unsigned int _z) : x(_x), z(_z) {}
};