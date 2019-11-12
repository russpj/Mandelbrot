#pragma once
#include "framework.h"

#include "Graphics.h"

class Calculator
{
public:
	Calculator(ColorMapper comapIn, ComplexMapper ptmapIn) :
		comap(comapIn), pointMapLowRes(ptmapIn)
	{
	}

	COLORREF MapPoint(int x, int y);

private:
	ColorMapper comap;
	ComplexMapper pointMapLowRes;
};


