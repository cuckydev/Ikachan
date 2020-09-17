#include "Opening.h"
#include "Draw.h"

void InitOpening(OPENING *op)
{
	op->x0 = 0;
	op->xC = -128;
	op->x8 = -64;
	op->x4 = SURFACE_WIDTH;
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
	PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 95, op->x0, &rcTitle, SURFACE_ID_OPENING);
	PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 28, (SURFACE_HEIGHT / 2) + 17, &rcCrab, SURFACE_ID_OPENING);
	PutBitmap3(&grcFull, op->x4, SURFACE_HEIGHT - 35, &rcPixel, SURFACE_ID_OPENING);

	for (int i = 0; i < ((SURFACE_HEIGHT + 63) / 64 + 1); ++i)
	{
		PutBitmap3(&grcFull, op->xC / 2, op->x8 + (i << 6), &rcLeft, 17);
		PutBitmap3(&grcFull, (SURFACE_WIDTH - 48) - op->xC / 2, op->x8 + (i << 6), &rcRight, 17);
	}

	//Move border and logo
	op->x8++;
	if (op->x0 < 45)
		op->x0++;
	if (op->x8 >= 0)
		op->x8 -= 64;
	if (op->xC / 2 < 0)
		op->xC++;

	//Move 'Made by Studio Pixel' text
	if (op->x4 <= ((SURFACE_WIDTH / 2) - 96))
	{
		//Draw 'Press Z' text
		if (op->x8 / 32 % 2)
			PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 32, (SURFACE_HEIGHT / 2) + 6, &rcPressZ, SURFACE_ID_OPENING);
	}
	else
	{
		op->x4 -= 2;
	}
}