#include "Graphics.h"

HBITMAP CreateGraphicalBitmap(HDC hdc, int width, int height)
{
	auto hdcDevice = CreateCompatibleDC(hdc);
	auto hbitmapCompatible = CreateCompatibleBitmap(hdcDevice, width, height);
	auto hbitmapOld = SelectObject(hdcDevice, hbitmapCompatible);

	if (hbitmapOld)
	{
		for (int x = 0; x < width; x += width / 10)
		{
			for (int y = 0; y < height; y += height / 10)
			{
				Rectangle(hdc, x, y, x + width / 10, y + height / 10);
			}
		}
		SelectObject(hdc, hbitmapOld);
	}

	return hbitmapCompatible;
}

void ComplexMapper::FitViewPort()
{
	double horizontalScale = (lrOriginal.real() - ulOriginal.real()) / width;
	double verticalScale = (ulOriginal.imag() - lrOriginal.imag()) / height;
	Complex viewPortMidPoint = (lrOriginal + ulOriginal) / 2.0;

	if (horizontalScale > verticalScale)
	{
		ulPaint = Complex(ulOriginal.real(), viewPortMidPoint.imag() + (ulOriginal.imag() - viewPortMidPoint.imag()) * horizontalScale / verticalScale);
		lrPaint = Complex(lrOriginal.real(), viewPortMidPoint.imag() + (lrOriginal.imag() - viewPortMidPoint.imag()) * horizontalScale / verticalScale);
	}
	else
	{
		ulPaint = Complex(viewPortMidPoint.real() + (ulOriginal.real() - viewPortMidPoint.real()) * verticalScale / horizontalScale, ulOriginal.imag());
		lrPaint = Complex(viewPortMidPoint.real() + (lrOriginal.real() - viewPortMidPoint.real()) * verticalScale / horizontalScale, lrOriginal.imag());
	}
}

Complex ComplexMapper::Map(int x, int y)
{
	double realPart = ulPaint.real() + x * (lrPaint.real() - ulPaint.real()) / width;
	double imagPart = ulPaint.imag() + y * (lrPaint.imag() - ulPaint.imag()) / height;
	return Complex(realPart, imagPart);
}

COLORREF InterpolateColors(COLORREF coMin, COLORREF coMax, int num, int den)
{
	auto red = GetRValue(coMin) + num * (GetRValue(coMax) - GetRValue(coMin)) / den;
	auto green = GetGValue(coMin) + num * (GetGValue(coMax) - GetGValue(coMin)) / den;
	auto blue = GetBValue(coMin) + num * (GetBValue(coMax) - GetBValue(coMin)) / den;
	return RGB(red, green, blue);
}

ColorMapper::ColorMapper(COLORREF coMin, COLORREF coMax, COLORREF coLast, int levels)
{
	for (int iter = 0; iter < levels-1; iter++)
	{
		COLORREF co = InterpolateColors(coMin, coMax, iter, levels);
		map.push_back(co);
	}
	map.push_back(coLast);
}

