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
short gMycLife[MAX_LEVEL] = { 4, 8, 12, 18, 26, 34, 62 };
short gMycExp[MAX_LEVEL] = { 8, 28, 52, 74, 102, 360, 852 };

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
	
	BYTE frame_no = (gMC.direct * 4) + gMC.ani_no;
	PutBitmap3(&grcFull, (gMC.x / 0x400) - (frame->x / 0x400) - 8, (gMC.y / 0x400) - (frame->y / 0x400) - 8, &rcMyChar[frame_no], SURFACE_ID_MYCHAR2);
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
		PlaySoundObject(1, 1);

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
			PlaySoundObject(6, 1);

		//Charge dash
		if (++gMC.dash_wait > 32)
			gMC.dash_wait = 32;
	}
	else if (gMC.dash_wait == 32)
	{
		//Play dash sound
		PlaySoundObject(7, 1);

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