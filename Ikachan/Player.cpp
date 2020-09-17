#include "Player.h"
#include "Draw.h"
#include "System.h"
#include "Sound.h"

MYCHAR gMC;

void PutMyChar()
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
	
	BYTE frame = (gMC.direct * 4) + gMC.ani_no;
	PutBitmap3(&grcFull, (gMC.x / 0x400) - 8, (gMC.y / 0x400) - 8, &rcMyChar[frame], SURFACE_ID_MYCHAR2);
}

int push_xm[3] = { 4294966784, 512, 0 };
int push_ym[3] = { 4294966784, 4294966784, 4294966572 };

int dash_xm[3] = { 4294964224, 3072, 0 };
int dash_ym[3] = { 0, 0, 4294964224 };

void ActMyChar_Normal(BOOL bKey)
{
	//Get direction
	gMC.direct = 2;
	if (gKey & KEY_LEFT)
		gMC.direct = 0;
	if (gKey & KEY_RIGHT)
		gMC.direct = 1;

	//Push
	if ((gKeyTrg & KEY_Z) && gMC.act_wait == 0)
	{
		//Push sound and particle
		PlaySoundObject(1, 1);

		//Play animation and disable for 8 frames
		gMC.act_wait = 8;
		gMC.ani_no = 1;
		if (gMC.ani_wait < 100)
			gMC.ani_wait += 10;

		//Push velocity
		gMC.xm += push_xm[gMC.direct];
		gMC.ym += push_ym[gMC.direct];
	}

	//Dash
	if ((gKey & KEY_Z) && TRUE)//(gMC.equip & 2))
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

	//Action timer
	if (gMC.act_wait > 0)
		--gMC.act_wait;

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
	
	if (FALSE)
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
}

void ActMyChar_Dash(BOOL bKey)
{
	//Decrease dash timer and stop dashing when depleted
	if (--gMC.dash_wait <= 0)
		gMC.unit = 0;
	
	//Dash particles
	
	//Move and use dash animation
	gMC.x += gMC.xm;
	gMC.y += gMC.ym;
	gMC.ani_no = 3;
}

typedef void (*MYCHAR_ACT)(BOOL);
void ActMyChar(BOOL bKey)
{
	static MYCHAR_ACT act[3] = {ActMyChar_Normal, ActMyChar_Dash, NULL};
	act[gMC.unit](bKey);
}

void InitMyChar()
{
	gMC.cond = 1;
	gMC.equip = 0;
	//byte_420DD9 = 0;
	//byte_420DF8 = 0;
	//word_420DF4 = word_41CA84[0];
	//word_420DF6 = 0;
	gMC.x = 0;//0xA0000;
	gMC.y = 0;//0x1A0000;
	gMC.ym = 0;
	gMC.xm = 0;
	//byte_420DFF = 1;
	gMC.ani_wait = 0;
	gMC.ani_no = 0;
	gMC.direct = 0;
	//byte_420E00 = 0;
	gMC.unit = 0;
	//word_420DF0 = 0;
	//word_420DF2 = 100;
	gMC.dash_wait = 0;
	gMC.act_wait = 0;
	//byte_420E01 = 0;
}