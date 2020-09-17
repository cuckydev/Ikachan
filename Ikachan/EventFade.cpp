#include "EventFade.h"
#include "Draw.h"
//#include "Frame.h"
#include "System.h"

#define FADE_WIDTH ((SURFACE_WIDTH + 15) / 16)
#define FADE_HEIGHT ((SURFACE_HEIGHT + 15) / 16)

BOOL ProcFade(FADE1 *fade)
{
	static RECT rcFade[16] = {
		{   0, 0,  16, 16 },
		{  16, 0,  32, 16 },
		{  32, 0,  48, 16 },
		{  48, 0,  64, 16 },
		{  64, 0,  80, 16 },
		{  80, 0,  96, 16 },
		{  96, 0, 112, 16 },
		{ 112, 0, 128, 16 },
		{ 128, 0, 144, 16 },
		{ 144, 0, 160, 16 },
		{ 160, 0, 176, 16 },
		{ 176, 0, 192, 16 },
		{ 192, 0, 208, 16 },
		{ 208, 0, 224, 16 },
		{ 224, 0, 240, 16 },
		{ 240, 0, 256, 16 },
	};

	switch (fade->mode)
	{
		case FM_NONE:
			fade->time = 0;
			break;
		case FM_QUAKE:
			if (++fade->time <= 60)
			{
				//Shake screen
				//frame->x += Random(-10, 10) << 10;
				//frame->y += Random(-10, 10) << 10;

				//Modify NPCs?
			}
			break;
		case FM_FADEOUT:
			for (int y = 0; y < FADE_HEIGHT; y++)
			{
				for (int x = 0; x < FADE_WIDTH; x++)
				{
					int frame = fade->time - y - x;
					if (frame < 0)
						frame = 0;
					if (frame > 15)
						frame = 15;
					PutBitmap3(&grcFull, x * 16, ((FADE_HEIGHT - 1) - y) * 16, &rcFade[frame], SURFACE_ID_FADE);
				}
			}
			if (++fade->time <= 50)
				break;
			fade->mode = 0;
			return TRUE;
		case FM_FADEIN:
			for (int y = 0; y < FADE_HEIGHT; y++)
			{
				for (int x = 0; x < FADE_WIDTH; x++)
				{
					int frame = fade->time - y - x;
					if (frame < 0)
						frame = 0;
					if (frame > 15)
						frame = 15;
					PutBitmap3(&grcFull, x * 16, ((FADE_HEIGHT - 1) - y) * 16, &rcFade[15 - frame], SURFACE_ID_FADE);
				}
			}
			if (++fade->time <= 50)
				break;
			fade->mode = 0;
			return TRUE;
		case FM_QUAKE2:
			if (!(++fade->time % 4))
			{
				//Shake screen
				//frame->x += Random(-10, 10) << 10;
				//frame->y += Random(-10, 10) << 10;

				//Modify NPCs?
			}
			break;
	}

	if (fade->mask)
		CortBox(&grcFull, 0x000000);
	return FALSE;
}