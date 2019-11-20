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
	std::vector<COLORREF> MapPoints(int x, int yMin, int yMax);

private:
	ColorMapper comap;
	ComplexMapper pointMapLowRes;
};


