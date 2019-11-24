#pragma once
#include "framework.h"

#include <vector>

#include "Graphics.h"

class Calculator
{
public:
	Calculator(ColorMapper comapIn, ComplexMapper ptmapIn) :
		comap(comapIn), pointMapLowRes(ptmapIn)
	{
	}

	COLORREF MapPoint(int x, int y);
	std::vector<COLORREF> MapPoints(int y, int xMin, int xMax);

	HBITMAP MapPoints(HDC hdc, RECT rect);

private:
	ColorMapper comap;
	ComplexMapper pointMapLowRes;
};


