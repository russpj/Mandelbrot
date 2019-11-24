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

vector<COLORREF> Calculator::MapPoints(int y, int xMin, int xMax)
{
	vector<COLORREF> results;
	for (auto x = xMin; x < xMax; x++)
	{
		results.push_back(MapPoint(x, y));
	}
	return results;
}

HBITMAP Calculator::MapPoints(HDC hdc, RECT rect)
{
	BITMAPINFO bmi;
	LPBYTE pBits;

	ZeroMemory(&bmi, sizeof(bmi));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = rect.right - rect.left;
	bmi.bmiHeader.biHeight = rect.bottom - rect.top;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	HBITMAP hbm = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, reinterpret_cast<void **>(&pBits), NULL, 0);

	for (auto x = rect.left; x < rect.right + 1; x++)
	{
		auto colors = MapPoints(x, rect.top, rect.bottom);
	}
	return hbm;
}

