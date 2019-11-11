#pragma once

// Some windows-specific graphics commands

#include "framework.h"

#include <complex>
using Complex = std::complex<double>;

#include <vector>

HBITMAP CreateGraphicalBitmap(HDC hdc, int width, int height);

class ComplexMapper
{
public:
	ComplexMapper(Complex ulIn, Complex lrIn, int widthIn, int heightIn) :
		ulOriginal(ulIn),
		lrOriginal(lrIn),
		width(widthIn),
		height(heightIn)
	{
		FitViewPort();
	}

	void FitViewPort();

	Complex Map(int x, int y);

private:
	Complex ulOriginal;
	Complex lrOriginal;
	Complex ulPaint;
	Complex lrPaint;
	int width;
	int height;
};

class ColorMapper
{
public:
	ColorMapper(COLORREF coMin, COLORREF coMax, COLORREF coSpecial, int levels);

	size_t size()
	{
		return map.size();
	}

	COLORREF Map(int index)
	{
		return map.at(index);
	}

private:
	std::vector<COLORREF> map;
};