#pragma once
#include <windows.h>

struct OPENING
{
	int title_y;
	int pixel_x;
	int border_y;
	int border_x;
};

void InitOpening(OPENING *op);
void PutOpening(OPENING *op);
