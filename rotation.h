#pragma once

enum rotation_type
{
	ROTATION_X = 0,
	ROTATION_Y = 1,
	ROTATION_Z = 2
};

struct rotation
{
	rotation_type type;
	float value;

	rotation() : type(ROTATION_X), value(0) {}
	rotation(rotation_type type, float value) : type(type), value(value) {}
};