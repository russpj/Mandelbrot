#include "framework.h"
#include "Mandelbrot.h"
#include <assert.h>
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

// Since windows bitmaps appear to work in row major order, so should this
vector<COLORREF> Calculator::MapPoints(int y, int xMin, int xMax)
{
	vector<COLORREF> results;
	for (auto x = xMin; x < xMax; x++)
	{
		results.push_back(MapPoint(x, y));
	}
	return results;
}

struct BitmapBits
{
	LPBYTE bits;
	int bytesWrittenInRow = 0;
	int bytesPerColor;

	BitmapBits(LPBYTE bitsIn, int bytesPerColorIn) : 
		bits(bitsIn), 
		bytesPerColor(bytesPerColorIn)
	{
		GdiFlush();
	}

	void WriteColor(COLORREF color)
	{
		assert(bytesPerColor == 3 || bytesPerColor == 4);
		LPBYTE start = bits;
		*bits++ = GetRValue(color);
		*bits++ = GetGValue(color);
		*bits++ = GetBValue(color);
		if (bytesPerColor == 4)
		{
			*bits++ = 0;
		}
		bytesWrittenInRow += (bits - start);
	}

	void EndScanLine()
	{
		if (bytesWrittenInRow % 2)
			*bits++ = 0;
		bytesWrittenInRow = 0;
	}
};

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


	if (hbm)
	{
		for (auto y = rect.top; y < rect.bottom; y++)
		{
			auto colors = MapPoints(y, rect.left, rect.right);

		}
	}

	return hbm;
}

