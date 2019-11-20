#include "framework.h"
#include "Mandelbrot.h"
#include <algorithm>

using std::vector;

#undef min
#undef max

int ValueCircle(Complex point, int levels)
{
	auto value = abs(point);
	auto mapValue = static_cast<int>(value * 100.0);
	mapValue = std::max(0, mapValue);
	mapValue = std::min(mapValue, levels-1);
	return mapValue;
}

int ValueMandelbrot(Complex point, int levels)
{
	int count = 0;
	auto z = point; // First iteration
	while (norm(z) < 4.0 && count < levels-1)
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

vector<COLORREF> Calculator::MapPoints(int x, int yMin, int yMax)
{
	vector<COLORREF> results;
	for (auto y = yMin; y < yMax; y++)
	{
		results.push_back(MapPoint(x, y));
	}
	return results;
}

