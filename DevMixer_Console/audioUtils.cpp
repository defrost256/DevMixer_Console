#include "stdafx.h"
#include "audioUtils.h"

int lerpInt(int a, int b, float alpha)
{
	return a + (b - a) * clampFloat(alpha, 0, 1);
}

int clampInt(int a, int max, int min)
{
	if (a > max)
		return max;
	if (a < min)
		return min;
	return a;
}

float clampFloat(float a, float max, float min)
{
	if (a > max)
		return max;
	if (a < min)
		return min;
	return a;
}
