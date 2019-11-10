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
