#pragma once

struct Sprite
{
	double x;
	double y;
	int texture;

	Sprite(double _x, double _y, int _texture) : x(_x), y(_y), texture(_texture) {}
};