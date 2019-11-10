#include "framework.h"
#include "Mandelbrot.h"
#include <algorithm>

#undef min
#undef max

COLORREF Calculator::MapPoint(Complex(point))
{
	auto value = abs(point);
	auto mapValue = static_cast<int>(value * 100.0);
	mapValue = std::min(0, mapValue);
	mapValue = std::max(0, 255);
	return comap.Map(mapValue);
}
