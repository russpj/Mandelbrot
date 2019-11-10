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
	double verticalScale = (lrOriginal.imag() - ulOriginal.imag()) / height;
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
	int red = GetRValue(coMin) + num * (GetRValue(coMax) - GetRValue(coMin)) / den;
	int blue = GetBValue(coMin) + num * (GetBValue(coMax) - GetBValue(coMin)) / den;
	int green = GetGValue(coMin) + num * (GetGValue(coMax) - GetGValue(coMin)) / den;
	return RGB(red, blue, green);
}

ColorMapper::ColorMapper()
{
	COLORREF coMin = RGB(200, 20, 0);
	COLORREF coMax = RGB(150, 150, 0);

	for (int iter = 0; iter < 256; iter++)
	{
		COLORREF co = InterpolateColors(coMin, coMax, iter, 256);
		map.push_back(co);
	}
}