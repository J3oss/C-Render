#pragma once

#include <color.h>

class Texture
{
public:
	virtual Color value(float u, float v) const { return Color(255,255,255,255); }
};
