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

int ValueMandelbrot(Complex point)
{
	int count = 0;
	auto z = point; // First iteration
	while (abs(z) < 2.0 && count < 255)
	{
		count++;
		z = z*z + point;
	}
	return count;
}

COLORREF Calculator::MapPoint(Complex point)
{
	return comap.Map(ValueMandelbrot(point));
}
