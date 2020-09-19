#include "Player.h"
#include "Draw.h"
#include "System.h"
#include "Sound.h"
#include "EventScript.h"

MYCHAR gMC;

//Unit act functions
void ActMyChar_Normal();
void ActMyChar_Dash();
void ActMyChar_Ship();

typedef void (*MYCHAR_ACT)();
MYCHAR_ACT act[3] = {ActMyChar_Normal, ActMyChar_Dash, ActMyChar_Ship};

//Life and exp per level
short gMycLife[MAX_LEVEL + 1] = { 4, 8, 12, 18, 26, 34, 62 };
short gMycExp[MAX_LEVEL + 1] = { 8, 28, 52, 74, 102, 360, 852 };

void DamageMyChar(char damage)
{
	//Check if we're invulnerable
	if (gMC.shock == 0)
	{
		//Take damage
		gMC.shock = 100;
		gMC.life -= damage;
		if (gMC.life < 0)
			gMC.life = 0;
		
		//Create damage indicator effect
		
		if (gMC.life != 0)
		{
			//Play hurt sound
			PlaySoundObject(SOUND_ID_OUCH, SOUND_MODE_PLAY);
		}
		else
		{
			//Die
			PlaySoundObject(SOUND_ID_DEAD, SOUND_MODE_PLAY);
			gMC.cond = FALSE;
			gMC.dead = TRUE;
			
			//Create death effect
		}
	}
}

BYTE JudgeHitMyCharBlock(int x, int y, char flag)
{
	//Collide with block
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

BYTE JudgeHitMyCharSnack(int x, int y, char flag, MAP *map)
{
	//Reset collision flag for some reason
	gMC.flag = 0;
	
	//Collide with block
	if ((flag & 1) && (flag & 2))
	{
		if ((gMC.x / 0x400) < (x * 16 + 15) && (gMC.y / 0x400) < (y * 16 + 12))
		{
			if (gMC.unit != 1)
				gMC.xm = 0;
			gMC.x = (x * 16 + 15) << 10;
			gMC.flag |= 1;
		}
		if ((gMC.y / 0x400) < (y * 16 + 15) && (gMC.x / 0x400) < (x * 16 + 12))
		{
			if (gMC.unit != 1)
				gMC.ym = 0;
			gMC.y = (y * 16 + 16) << 10;
			gMC.flag |= 2;
		}
	}
	
	if ((flag & 4) && (flag & 2))
	{
		if (((gMC.x + 0x3FF) / 0x400) > (x * 16 - 14) && (gMC.y / 0x400) < (y * 16 + 12))
		{
			if (gMC.unit != 1)
				gMC.xm = 0;
			gMC.x = (x * 16 - 14) << 10;
			gMC.flag |= 4;
		}
		if ((gMC.y / 0x400) < (y * 16 + 15) && (gMC.x / 0x400) > (x * 16 - 12))
		{
			if (gMC.unit != 1)
				gMC.ym = 0;
			gMC.y = (y * 16 + 16) << 10;
			gMC.flag |= 2;
		}
	}
	
	if ((flag & 1) && (flag & 8))
	{
		if ((gMC.x / 0x400) < (x * 16 + 15) && (gMC.y / 0x400) > (y * 16 - 12))
		{
			if (gMC.unit != 1)
				gMC.xm = 0;
			gMC.x = (x * 16 + 15) << 10;
			gMC.ym = 0;
			gMC.flag |= 1u;
		}
		if ((gMC.y / 0x400) >= (y * 16 - 16) && (gMC.x / 0x400) < (x * 16 + 12))
		{
			if (gMC.unit != 1)
			{
				gMC.airborne = FALSE;
				if (gMC.ym > 0)
					gMC.ym = 0;
			}
			gMC.y = (y * 16 - 16) << 10;
			gMC.flag |= 8;
		}
	}
	
	if ((flag & 4) && (flag & 8))
	{
		if (((gMC.x + 0x3FF) / 0x400) > (x * 16 - 14) && (gMC.y / 0x400) > (y * 16 - 12))
		{
			if (gMC.unit != 1)
				gMC.xm = 0;
			gMC.x = (x * 16 - 14) << 10;
			gMC.flag |= 4;
		}
		if ((gMC.y / 0x400) >= (y * 16 - 16) && (gMC.x / 0x400) > (x * 16 - 12))
		{
			if (gMC.unit != 1)
			{
				gMC.airborne = FALSE;
				if (gMC.ym > 0)
					gMC.ym = 0;
			}
			gMC.y = (y * 16 - 16) << 10;
			gMC.flag |= 8;
		}
	}
	
	//Break block
	if ((gMC.unit == 1 && gMC.flag != 8 && gMC.flag) || ((gMC.equip & 1) && (gMC.flag & 2)))
	{
		//Play break sound, remove block, and create effect
		PlaySoundObject(SOUND_ID_CRASH, SOUND_MODE_PLAY);
		map->data[x + map->width * y] = 0;
	}
	return gMC.flag;
}

void JudgeHitMyCharDamage(int x, int y)
{
	if ((gMC.x / 0x400) < (x * 16 + 10) &&
		(gMC.x / 0x400) > (x * 16 - 10) &&
		(gMC.y / 0x400) < (y * 16 + 10) &&
		(gMC.y / 0x400) > (y * 16 - 10))
	{
		//Take damage
		DamageMyChar(3);
	}
}

void JudgeHitMyCharVector(int x, int y, char type)
{
	if ((gMC.x / 0x400) < (x * 16 + 8) &&
		(gMC.x / 0x400) >= (x * 16 - 8) &&
		(gMC.y / 0x400) < (y * 16 + 8) &&
		(gMC.y / 0x400) >= (y * 16 - 8))
	{
		switch (type)
		{
			case 0x60:
				gMC.xm -= 50;
				break;
			case 0x61:
				gMC.xm += 50;
				break;
			case 0x62:
				gMC.ym -= 50;
				break;
			case 0x63:
				gMC.ym += 50;
				break;
			default:
				return;
		}
	}
}

void JudgeHitMyCharItem(int x, int y, MAP *map)
{
	if ((gMC.x / 0x400) < (x * 16 + 8) &&
		(gMC.x / 0x400) > (x * 16 - 8) &&
		(gMC.y / 0x400) < (y * 16 + 8) &&
		(gMC.y / 0x400) > (y * 16 - 8))
	{
		//Remove item, play item sound, add exp and life, and create effects
		map->data[x + map->width * y] = 0;
		PlaySoundObject(SOUND_ID_ITEM, SOUND_MODE_PLAY);
		gMC.life++;
		gMC.exp++;
		if (gMC.life > gMycLife[gMC.level])
			gMC.life = gMycLife[gMC.level];
	}
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

	v13[0] = 1 | 2;
	v13[1] = 4 | 2;
	v13[2] = 8 | 1;
	v13[3] = 8 | 4;
	
	//Collision position
	int x = gMC.x / 0x400 / 16;
	int y = gMC.y / 0x400 / 16;

	//Reset collision state
	char v19 = 4;
	gMC.flag = 0;
	
	for (int i = 0; i < 4; i++)
	{
		//Get tile at the position to check
		int part = map->data[(x + offx[i]) + map->width * (y + offy[i])];

		//Block collision
		if (part < 0x80 || part >= 0xA0)
			v19 -= v7[i];
		else if (!(JudgeHitMyCharBlock(x + offx[i], y + offy[i], v13[i]) & 8))
			v19 -= v7[i];
		//Vector collision
		if (part >= 0x60 && part < 0x80)
			JudgeHitMyCharVector(x + offx[i], y + offy[i], part);
		//Damage collision
		if (part >= 0xA0 && part < 0xC0)
			JudgeHitMyCharDamage(x + offx[i], y + offy[i]);
		//Item collision
		else if (part >= 0x40 && part < 0x60)
			JudgeHitMyCharItem(x + offx[i], y + offy[i], map);
		//Snack collision
		else if (part < 0xE0 || part >= 0x100)
			v19 -= v7[i];
		else if (!(JudgeHitMyCharSnack(x + offx[i], y + offy[i], v13[i], map) & 8))
			v19 -= v7[i];
	}
	
	//Check if we should level up
	if (gMC.exp >= gMycExp[gMC.level])
	{
		//Level up
		gMC.exp -= gMycExp[gMC.level];
		++gMC.level;
		if (gMC.level > MAX_LEVEL)
			gMC.level = MAX_LEVEL;
		else
			PlaySoundObject(SOUND_ID_LEVELUP, SOUND_MODE_PLAY);
	}
	
	//Set airborne flag
	if (v19 <= 0)
		gMC.airborne = TRUE;
}

//Draw MyChar
void PutMyChar(FRAME *frame)
{
	static RECT rcMyChar[12] = {
		{  0,  0, 16, 16 },
		{ 16,  0, 32, 16 },
		{ 32,  0, 48, 16 },
		{ 48,  0, 64, 16 },

		{  0, 16, 16, 32 },
		{ 16, 16, 32, 32 },
		{ 32, 16, 48, 32 },
		{ 48, 16, 64, 32 },

		{  0, 32, 16, 48 },
		{ 16, 32, 32, 48 },
		{ 32, 32, 48, 48 },
		{ 48, 32, 64, 48 },
	};

	static RECT rcMyCharShip[12] = {
		{   0,  0,  40,  40 },
		{  40,  0,  80,  40 },
		{  80,  0, 120,  40 },
		{ 120,  0, 160,  40 },

		{   0, 40,  40,  80 },
		{  40, 40,  80,  80 },
		{  80, 40, 120,  80 },
		{ 120, 40, 160,  80 },

		{   0, 80,  40, 120 },
		{  40, 80,  80, 120 },
		{  80, 80, 120, 120 },
		{ 120, 80, 160, 120 },
	};
	
	BYTE frame_no = (gMC.direct * 4) + gMC.ani_no;
	if (gMC.equip & 8)
		PutBitmap3(&grcFull, (gMC.x / 0x400) - (frame->x / 0x400) - 12, (gMC.y / 0x400) - (frame->y / 0x400) - 12, &rcMyCharShip[frame_no], SURFACE_ID_MYCHAR3);
	else if (gMC.equip & 1)
		PutBitmap3(&grcFull, (gMC.x / 0x400) - (frame->x / 0x400), (gMC.y / 0x400) - (frame->y / 0x400), &rcMyChar[frame_no], SURFACE_ID_MYCHAR);
	else
		PutBitmap3(&grcFull, (gMC.x / 0x400) - (frame->x / 0x400), (gMC.y / 0x400) - (frame->y / 0x400), &rcMyChar[frame_no], SURFACE_ID_MYCHAR2);
}

void PutMyStatus()
{
	static RECT rcStatus = { 0, 0, 88, 32 };
	PutBitmap3(&grcFull, 8, 8, &rcStatus, SURFACE_ID_STATUS);
	PutNumber(16, 8, gMC.level);
	PutNumber(16, 16, gMC.exp);
	PutNumber(48, 16, gMycExp[gMC.level]);
	PutNumber(16, 24, gMC.life);
	PutNumber(48, 24, gMycLife[gMC.level]);
}

//Update MyChar
int swim_xm[3] = { -0x200, 0x200, 0 };
int swim_ym[3] = { -0x200, -0x200, -0x2D4 };

int dash_xm[3] = { -0xC00, 0xC00, 0 };
int dash_ym[3] = { 0, 0, -0xC00 };

void ActMyChar_Normal()
{
	//Get direction
	gMC.direct = 2;
	if (gKey & KEY_LEFT)
		gMC.direct = 0;
	if (gKey & KEY_RIGHT)
		gMC.direct = 1;

	//Swim
	if ((gKeyTrg & KEY_Z) && gMC.swim_wait == 0)
	{
		//Swim sound and particle
		PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);

		//Play animation and disable for 8 frames
		gMC.swim_wait = 8;
		gMC.ani_no = 1;
		if (gMC.ani_wait < 100)
			gMC.ani_wait += 10;

		//Swim velocity
		gMC.xm += swim_xm[gMC.direct];
		gMC.ym += swim_ym[gMC.direct];
	}

	//Dash
	if ((gKey & KEY_Z) && (gMC.equip & 2))
	{
		//Play charged sound
		if (gMC.dash_wait == 31)
			PlaySoundObject(SOUND_ID_READY, SOUND_MODE_PLAY);

		//Charge dash
		if (++gMC.dash_wait > 32)
			gMC.dash_wait = 32;
	}
	else if (gMC.dash_wait == 32)
	{
		//Play dash sound
		PlaySoundObject(SOUND_ID_GO, SOUND_MODE_PLAY);

		//Dash particle

		//Dash velocity and set to dash mode
		gMC.xm = dash_xm[gMC.direct];
		gMC.ym = dash_ym[gMC.direct];
		gMC.unit = 1;
	}
	else
	{
		gMC.dash_wait = 0;
	}
	
	//Animation timer
	if (gMC.ani_wait <= 0)
		gMC.ani_no = 0;
	else
		--gMC.ani_wait;

	//Swim timer
	if (gMC.swim_wait > 0)
		--gMC.swim_wait;

	//Dash animation
	if (gMC.dash_wait == 32)
		gMC.ani_no = 2;
	
	//Gravity
	if (gMC.ym < 0x800)
		gMC.ym += 20;
	
	//Speed limit
	if (gMC.ym < -0x800)
		gMC.ym = -0x800;
	if (gMC.ym > 0x800)
		gMC.ym = 0x800;
	if (gMC.xm < -0x800)
		gMC.xm = -0x800;
	if (gMC.xm > 0x800)
		gMC.xm = 0x800;
	
	//Friction
	if (gMC.xm > 0)
		gMC.xm -= 8;
	if (gMC.xm < 0)
		gMC.xm += 8;
	
	if (!gMC.airborne)
	{
		if (gMC.xm > 0)
			gMC.xm -= 24;
		if (gMC.xm < 0)
			gMC.xm += 24;
	}
	
	//Move
	if (gMC.xm >= 8 || gMC.xm <= -8)
		gMC.x += gMC.xm;
	gMC.y += gMC.ym;

	//Decrement timers
	if (gMC.shock)
		--gMC.shock;
	if (gMC.no_event)
		--gMC.no_event;
}

void ActMyChar_Dash()
{
	//Decrease dash timer and stop dashing when depleted
	if (--gMC.dash_wait <= 0)
		gMC.unit = 0;
	
	//Dash particles
	
	//Move and use dash animation
	gMC.x += gMC.xm;
	gMC.y += gMC.ym;
	gMC.ani_no = 3;

	//Decrement timers
	if (gMC.shock)
		--gMC.shock;
}

void ActMyChar_Ship()
{
	
}

void ActMyChar()
{
	act[gMC.unit]();
}

//Initalize MyChar
void InitMyChar()
{
	gMC.cond = 1;
	gMC.equip = 0;
	gMC.dead = 0;
	gMC.level = 0;
	gMC.life = gMycLife[0];
	gMC.exp = 0;
	gMC.x = 0xA0000;
	gMC.y = 0x1A0000;
	gMC.ym = 0;
	gMC.xm = 0;
	gMC.airborne = 1;
	gMC.ani_wait = 0;
	gMC.ani_no = 0;
	gMC.direct = 0;
	gMC.flag = 0;
	gMC.unit = 0;
	gMC.shock = 0;
	gMC.no_event = 100;
	gMC.dash_wait = 0;
	gMC.swim_wait = 0;
	gMC.carry = 0;
}
