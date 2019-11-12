#include "framework.h"
#include "Mandelbrot.h"
#include <algorithm>

#undef min
#undef max

int ValueCircle(Complex point)
{
	auto value = abs(point);
	auto mapValue = static_cast<int>(value * 100.0);
	mapValue = std::max(0, mapValue);
	mapValue = std::min(mapValue, 255);
	return mapValue;
}

int ValueMandelbrot(Complex point, int levels)
{
	int count = 0;
	auto z = point; // First iteration
	while (abs(z) < 2.0 && count < levels-1)
	{
		count++;
		z = z*z + point;
	}
	return count;
}

COLORREF Calculator::MapPoint(int x, int y)
{
	auto point = pointMapLowRes.Map(x, y);
	return comap.Map(ValueMandelbrot(point, comap.size()));
}
