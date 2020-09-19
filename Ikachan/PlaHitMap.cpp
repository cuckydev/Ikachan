#include "PlaHitMap.h"
#include "Player.h"
#include "Sound.h"

BYTE JudgeHitMyCharBlock(int x, int y, char flag)
{
	if ((flag & 1) && (flag & 2))
	{
		if ((gMC.x / 0x400) < (x * 16 + 15) && (gMC.y / 0x400) < (y * 16 + 12))
		{
			if (gMC.xm < -0x400)
				PlaySoundObject(SOUND_ID_HITHEAD, SOUND_MODE_PLAY);
			gMC.x = (x * 16 + 15) << 10;
			gMC.xm = 0;
			gMC.flag |= 1;
		}
		if ((gMC.y / 0x400) < (y * 16 + 15) && (gMC.x / 0x400) < (x * 16 + 12))
		{
			if (gMC.ym < -0x400)
				PlaySoundObject(SOUND_ID_HITHEAD, SOUND_MODE_PLAY);
			gMC.y = (y * 16 + 16) << 10;
			gMC.ym = 0;
			gMC.flag |= 2;
		}
	}
	
	if ((flag & 4) && (flag & 2))
	{
		if (((gMC.x + 0x3FF) / 0x400) > (x * 16 - 14) && (gMC.y / 0x400) < (y * 16 + 12))
		{
			if (gMC.xm < -0x400)
				PlaySoundObject(SOUND_ID_HITHEAD, SOUND_MODE_PLAY);
			gMC.x = (x * 16 - 14) << 10;
			gMC.xm = 0;
			gMC.flag |= 4;
		}
		if ((gMC.y / 0x400) < (y * 16 + 15) && (gMC.x / 0x400) > (x * 16 - 12))
		{
			if (gMC.ym < -0x400)
				PlaySoundObject(SOUND_ID_HITHEAD, SOUND_MODE_PLAY);
			gMC.y = (y * 16 + 16) << 10;
			gMC.ym = 0;
			gMC.flag |= 2;
		}
	}

	if ((flag & 1) && (flag & 8))
	{
		if ((gMC.x / 0x400) < (x * 16 + 15) && (gMC.y / 0x400) > (y * 16 - 12))
		{
			if (gMC.xm < -0x400)
				PlaySoundObject(SOUND_ID_HITHEAD, SOUND_MODE_PLAY);
			gMC.x = (x * 16 + 15) << 10;
			gMC.xm = 0;
			gMC.flag |= 1u;
		}
		if ((gMC.y / 0x400) >= (y * 16 - 16) && (gMC.x / 0x400) < (x * 16 + 12))
		{
			gMC.airborne = FALSE;
			gMC.y = (y * 16 - 16) << 10;
			if (gMC.ym > 0)
				gMC.ym = 0;
			gMC.flag |= 8;
		}
	}
	
	if ((flag & 4) && (flag & 8))
	{
		if (((gMC.x + 0x3FF) / 0x400) > (x * 16 - 14) && (gMC.y / 0x400) > (y * 16 - 12))
		{
			if (gMC.xm > 0x400)
				PlaySoundObject(SOUND_ID_HITHEAD, SOUND_MODE_PLAY);
			gMC.x = (x * 16 - 14) << 10;
			gMC.xm = 0;
			gMC.flag |= 4;
		}
		if ((gMC.y / 0x400) >= (y * 16 - 16) && (gMC.x / 0x400) > (x * 16 - 12))
		{
			gMC.airborne = FALSE;
			gMC.y = (y * 16 - 16) << 10;
			if (gMC.ym > 0)
				gMC.ym = 0;
			gMC.flag |= 8;
		}
	}
	return gMC.flag;
}

void HitMyCharMap(MAP *map)
{
	//Collision offsets
	char offx[4];
	char offy[4];
	char v7[4];
	char v13[4];
	
	offx[0] = 0;
	offx[1] = 1;
	offx[2] = 0;
	offx[3] = 1;
	
	offy[0] = 0;
	offy[1] = 0;
	offy[2] = 1;
	offy[3] = 1;

	v7[0] = 0;
	v7[1] = 0;
	v7[2] = 1;
	v7[3] = 1;

	v13[0] = 0x3;
	v13[1] = 0x6;
	v13[2] = 0x9;
	v13[3] = 0xC;
	
	//Collision position
	int x = gMC.x / 0x400 / 16;
	int y = gMC.y / 0x400 / 16;

	//Reset collision state
	char v19 = 1;
	gMC.flag = 0;
	
	for (int i = 0; i < 4; i++)
	{
		//Get tile at the position to check
		BYTE part = map->data[(x + offx[i]) + map->width * (y + offy[i])];

		//Block collision
		if (part < 0x80 || part >= 0xA0)
			v19 -= v7[i];
		else if (!JudgeHitMyCharBlock(x + offx[i], y + offy[i], v13[i]))
			v19 -= v7[i];
	}
	
	//Set airborne flag
	if (v19 <= 0)
		gMC.airborne = TRUE;
}
