#include "Opening.h"
#include "Draw.h"

void InitOpening(OPENING *op)
{
	op->title_y = -83;
	op->border_x = -128;
	op->border_y = -64;
	op->pixel_x = SURFACE_WIDTH;
}

void PutOpening(OPENING *op)
{
	static RECT rcUnused = { 0, 0, 32, 32 };
	static RECT rcLeft = { 0, 0, 48, 64 };
	static RECT rcRight = { 0, 64, 48, 128 };
	static RECT rcTitle = { 48, 0, 248, 64 };
	static RECT rcCrab = { 248, 0, 320, 72 };
	static RECT rcPixel = { 48, 64, 248, 88 };
	static RECT rcPressZ = { 248, 72, 320, 80 };
	
	//Draw main title elements
	CortBox(&grcFull, 0xFFFFFF);
	PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 95, op->title_y, &rcTitle, SURFACE_ID_OPENING);
	PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 28, (SURFACE_HEIGHT / 2) + 17, &rcCrab, SURFACE_ID_OPENING);
	PutBitmap3(&grcFull, op->pixel_x, SURFACE_HEIGHT - 35, &rcPixel, SURFACE_ID_OPENING);
	
	for (int i = 0; i < ((SURFACE_HEIGHT + 63) / 64 + 1); ++i)
	{
		PutBitmap3(&grcFull, op->border_x / 2, op->border_y + (i << 6), &rcLeft, 17);
		PutBitmap3(&grcFull, (SURFACE_WIDTH - 48) - op->border_x / 2, op->border_y + (i << 6), &rcRight, 17);
	}
	
	//Move border and logo
	op->border_y++;
	if (op->title_y < 45)
		op->title_y++;
	if (op->border_y >= 0)
		op->border_y -= 64;
	if (op->border_x / 2 < 0)
		op->border_x++;
	
	if (op->pixel_x > ((SURFACE_WIDTH / 2) - 96))
	{
		//Move 'Made by Studio Pixel' text
		op->pixel_x -= 2;
	}
	else
	{
		//Draw 'Press Z' text once that's stopped moving
		if (op->border_y / 32 % 2)
			PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 32, (SURFACE_HEIGHT / 2) + 6, &rcPressZ, SURFACE_ID_OPENING);
	}
	
}