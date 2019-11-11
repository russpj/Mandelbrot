#pragma once
#include "framework.h"

#include "Graphics.h"

class Calculator
{
public:
	Calculator(ColorMapper map) :
		comap(map)
	{
	}

	COLORREF MapPoint(Complex(point));

private:
	ColorMapper comap;
};


