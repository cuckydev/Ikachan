#include "Boss.h"
#include "Draw.h"
#include "Player.h"
#include "Sound.h"

BOSS gBoss = { 0 };

void InitBoss()
{
	gBoss.cond = TRUE;
	gBoss.life = 15;
	gBoss.shock = 0;
	gBoss.x6 = 0;
	gBoss.code_char = 0;
	gBoss.code_event = 1000;
	gBoss.defeat_event = 1001;
	gBoss.act_wait = 0;
	gBoss.act_no = 0;
	gBoss.ani_no = 0;
	gBoss.ani_wait = 0;
	gBoss.direct = 0;
	gBoss.x = 0x20C000;
	gBoss.y = 0x168000;
	gBoss.xm = 0;
	gBoss.ym = 0;
	gBoss.tgt_x = 0x218000;
	gBoss.tgt_y = 0x16C000;
	gBoss.hit.left = 0x4000;
	gBoss.hit.top = 0x800;
	gBoss.hit.right = 0xC000;
	gBoss.hit.bottom = 0x5800;
}

void PutBoss(FRAME *frame)
{
	static RECT rcIronHead[10] =
	{
		{  0,  0, 32, 24 },
		{  0, 72, 32, 96 },
		{ 32,  0, 64, 24 },
		{  0, 24, 32, 48 },
		{  0, 48, 32, 72 },
		{  0, 24, 32, 48 },
		{ 32,  0, 64, 24 },
		{ 32, 24, 64, 48 },
		{ 32, 48, 64, 72 },
		{ 32, 24, 64, 48 },
	};
	
	if (gBoss.cond)
	{
		//Get head rect
		RECT head;
		if (gBoss.act_no == 2)
			head = rcIronHead[gBoss.act_wait % 2];
		else
			head = rcIronHead[0];
		
		//Get body rect
		RECT body = rcIronHead[gBoss.ani_no];
		
		//Draw Iron Head
		if (gBoss.direct)
		{
			head.left += 64;
			head.right += 64;
			body.left += 64;
			body.right += 64;
			PutBitmap3(&grcFull, 
			           (gBoss.x / 0x400) - (frame->x / 0x400) + 32,
					   (gBoss.y / 0x400) - (frame->y / 0x400),
					   &head,
					   SURFACE_ID_IRONHEAD + gBoss.code_char);
			PutBitmap3(&grcFull,
			           (gBoss.x / 0x400) - (frame->x / 0x400),
					   (gBoss.y / 0x400) - (frame->y / 0x400),
					   &body,
					   SURFACE_ID_IRONHEAD + gBoss.code_char);
		}
		else
		{
			PutBitmap3(&grcFull, 
			           (gBoss.x / 0x400) - (frame->x / 0x400),
					   (gBoss.y / 0x400) - (frame->y / 0x400),
					   &head,
					   SURFACE_ID_IRONHEAD + gBoss.code_char);
			PutBitmap3(&grcFull,
			           (gBoss.x / 0x400) - (frame->x / 0x400) + 32,
					   (gBoss.y / 0x400) - (frame->y / 0x400),
					   &body,
					   SURFACE_ID_IRONHEAD + gBoss.code_char);
		}
	}
}

void ActBoss(CARET_SPAWNER *caret_spawner)
{
	int acc_x, acc_y;
	
	//Decrement shock counter
	if (gBoss.shock > 0)
		--gBoss.shock;
	
	switch (gBoss.act_no)
	{
		case 0:
			//Move towards target
			acc_x = 6;
			acc_y = 4;
			
			if (gBoss.x > gBoss.tgt_x)
				gBoss.xm -= acc_x;
			if (gBoss.x < gBoss.tgt_x)
				gBoss.xm += acc_x;
			if (gBoss.y > gBoss.tgt_y)
				gBoss.ym -= acc_y;
			if (gBoss.y < gBoss.tgt_y)
				gBoss.ym += acc_y;
			
			//Swim in moving direction
			if (gBoss.xm < 0 && gBoss.direct == 1)
			{
				//Turn around with bubble effect
				gBoss.direct = 0;
				int bubble_i = FindCaretSpawner(caret_spawner);
				if (bubble_i != NO_CARET)
				{
					CARET_SPAWNER *caretsp = &caret_spawner[bubble_i];
					caretsp->cond = TRUE;
					caretsp->type = 1;
					caretsp->ani_no = 0;
					caretsp->num = 5;
					caretsp->x = gBoss.x + 0xC000;
					caretsp->y = gBoss.y + 0x3000;
					caretsp->rand_moveright = 0x400;
					caretsp->rand_moveleft = 0x200;
					caretsp->rand_movedown = 0x400;
					caretsp->rand_moveup = -0x400;
					caretsp->rand_x = 16;
					caretsp->rand_y = 12;
				}
			}
			if (gBoss.xm > 0 && gBoss.direct == 0)
			{
				//Turn around with bubble effect
				gBoss.direct = 1;
				int bubble_i = FindCaretSpawner(caret_spawner);
				if (bubble_i != NO_CARET)
				{
					CARET_SPAWNER *caretsp = &caret_spawner[bubble_i];
					caretsp->cond = TRUE;
					caretsp->type = 1;
					caretsp->ani_no = 0;
					caretsp->num = 5;
					caretsp->x = gBoss.x + 0x4000;
					caretsp->y = gBoss.y + 0x3000;
					caretsp->rand_moveright = -0x200;
					caretsp->rand_moveleft = -0x400;
					caretsp->rand_movedown = 0x400;
					caretsp->rand_moveup = -0x400;
					caretsp->rand_x = 16;
					caretsp->rand_y = 12;
				}
			}
			
			//Animate
			if (++gBoss.ani_wait > 8)
			{
				gBoss.ani_wait = 0;
				gBoss.ani_no++;
				if (gBoss.ani_no > 9)
					gBoss.ani_no = 2;
			}
			
			//Limit speed
			if (gBoss.xm > 0x800)
				gBoss.xm = 0x800;
			if (gBoss.xm < -0x800)
				gBoss.xm = -0x800;
			if (gBoss.ym > 0x800)
				gBoss.ym = 0x800;
			if (gBoss.ym < -0x800)
				gBoss.ym = -0x800;
			
			//Move
			gBoss.x += gBoss.xm;
			gBoss.y += gBoss.ym;
			break;
		case 1:
			//Move towards Ikachan
			acc_x = 8;
			acc_y = 8;
			
			if (gBoss.x > gMC.x)
				gBoss.xm -= acc_x;
			if (gBoss.x < gMC.x)
				gBoss.xm += acc_x;
			if (gBoss.y > gMC.y)
				gBoss.ym -= acc_y;
			if (gBoss.y < gMC.y)
				gBoss.ym += acc_y;
			
			//Charge towards Ikachan after some time
			if (gBoss.act_wait > 0)
				gBoss.act_wait--;
			if (gBoss.act_wait <= 0 && gBoss.y < gMC.y && (gBoss.y + 0x2000) > gMC.y)
			{
				gBoss.act_wait = 400;
				gBoss.ym = 0;
				gBoss.act_no = 2;
				if ((gBoss.x + 0x6000) <= gMC.x)
					gBoss.xm = 0x4000;
				else
					gBoss.xm = -0x4000;
			}
			
			//Face in moving direction
			if (gBoss.xm < 0 && gBoss.direct == 1)
				gBoss.direct = 0;
			if (gBoss.xm > 0 && gBoss.direct == 0)
				gBoss.direct = 1;
			
			//Animate
			if (++gBoss.ani_wait > 4)
			{
				gBoss.ani_wait = 0;
				gBoss.ani_no++;
			}
			if (gBoss.ani_no > 9)
				gBoss.ani_no = 2;
			
			//Limit speed
			if (gBoss.xm > 0x800)
				gBoss.xm = 0x800;
			if (gBoss.xm < -0x800)
				gBoss.xm = -0x800;
			if (gBoss.ym > 0x800)
				gBoss.ym = 0x800;
			if (gBoss.ym < -0x800)
				gBoss.ym = -0x800;
			
			//Move
			gBoss.x += gBoss.xm;
			gBoss.y += gBoss.ym;
			break;
		case 2:
			//Slow down and stop charging once velocity is near zero
			if (gBoss.xm > 0)
				gBoss.xm -= 8;
			if (gBoss.xm < 0)
				gBoss.xm += 8;
			if (gBoss.xm < 8 && gBoss.xm > -8)
				gBoss.act_no = 1;
			
			//Decrement charge timer
			if (gBoss.act_wait > 0)
				gBoss.act_wait--;
			
			//Charge effect
			if ((gBoss.act_wait % 5) == 0)
			{
				int star_i = FindCaretSpawner(caret_spawner);
				if (star_i != NO_CARET)
				{
					CARET_SPAWNER *caretsp = &caret_spawner[star_i];
					caretsp->cond = TRUE;
					caretsp->type = 0;
					caretsp->ani_no = 0;
					caretsp->num = 1;
					caretsp->x = gBoss.x + 0x8000;
					caretsp->y = gBoss.y + 0x3000;
					caretsp->rand_moveright = gBoss.xm * -1;
					caretsp->rand_moveleft = gBoss.xm * -1;
					caretsp->rand_movedown = 0x400;
					caretsp->rand_moveup = -0x244;
					caretsp->rand_x = 16;
					caretsp->rand_y = 12;
				}
			}
			
			//Set animation and move
			gBoss.ani_no = 2;
			gBoss.x += gBoss.xm;
			gBoss.y += gBoss.ym;
			break;
	}
}

void DamageBoss(CARET_SPAWNER *caret_spawner, char damage)
{
	if (gBoss.shock == 0)
	{
		//Damage Iron Head
		gBoss.shock = 100;
		gBoss.life -= damage;
		if (gBoss.life < 0)
			gBoss.life = 0;
		
		//Create damage indicator
		int dmg_i = FindCaretSpawner(caret_spawner);
		if (dmg_i != NO_CARET)
		{
			CARET_SPAWNER *caretsp = &caret_spawner[dmg_i];
			caretsp->cond = TRUE;
			caretsp->type = 2;
			caretsp->ani_no = 10 - damage;
			caretsp->num = 1;
			caretsp->x = gBoss.x + 0x2000;
			caretsp->y = gBoss.y - 0x1000;
			caretsp->rand_x = 1;
			caretsp->rand_y = 0;
		}
		
		//Play hurt sound
		PlaySoundObject(20, 1);
	}
}

void HitMyCharBoss(EVENT_SCR *event_scr, CARET_SPAWNER *caret_spawner)
{
	char flag = 0;
	char touch = FALSE; //???
	
	if (gBoss.cond == FALSE)
		return;
	
	//Check if Ikachan is touching Iron Head
	if (gMC.x < (gBoss.x + gBoss.hit.right) &&
		gMC.x > (gBoss.x + gBoss.hit.right - 0x1400) &&
		gMC.y < (gBoss.y + gBoss.hit.bottom - 0x1000) &&
		(gMC.y + 0x4000) > (gBoss.y + gBoss.hit.top + 0x1000))
	{
		gMC.x = gBoss.x + gBoss.hit.right;
		gMC.xm = 0;
		flag |= 1;
		touch = TRUE;
	}
	if (gMC.y < (gBoss.y + gBoss.hit.bottom) &&
		gMC.y > (gBoss.y + gBoss.hit.bottom - 0x1400) &&
		gMC.x < (gBoss.x + gBoss.hit.right - 0x1000) &&
		(gMC.x + 0x4000) > (gBoss.x + gBoss.hit.left + 0x1000))
	{
		if (gMC.ym < -100)
			PlaySoundObject(SOUND_ID_HITHEAD, SOUND_MODE_PLAY);
		gMC.y = gBoss.y + gBoss.hit.bottom;
		gMC.ym = 0;
		flag |= 2;
		touch = TRUE;
	}
	if ((gMC.x + 0x4000) > (gBoss.x + gBoss.hit.left) &&
		(gMC.x + 0x4000) < (gBoss.x + gBoss.hit.left + 0x1400) &&
		gMC.y < (gBoss.y + gBoss.hit.bottom - 0x1000) &&
		(gMC.y + 0x4000) > (gBoss.y + gBoss.hit.top + 0x1000))
	{
		gMC.x = gBoss.x + gBoss.hit.left - 0x4000;
		gMC.xm = 0;
		flag |= 4;
		touch = TRUE;
	}
	if ((gMC.y + 0x4000) > (gBoss.y + gBoss.hit.top) &&
		(gMC.y + 0x4000) < (gBoss.y + gBoss.hit.top + 0x1400) &&
		gMC.x < (gBoss.x + gBoss.hit.right - 0x1000) &&
		(gMC.x + 0x4000) > (gBoss.x + gBoss.hit.left + 0x1000))
	{
		gMC.airborne = FALSE;
		gMC.y = gBoss.y + gBoss.hit.top - 0x4000;
		if (gMC.ym > 0)
			gMC.ym = 0;
		flag |= 8;
		touch = TRUE;
	}
	
	if (touch)
	{
		//Handle interaction
		switch (gBoss.act_no)
		{
			case 0:
				if (gMC.no_event == 0)
				{
					event_scr->mode = 1;
					event_scr->x1C = 4;
					event_scr->event_no = gBoss.code_event;
					gMC.no_event = 100;
				}
				break;
			case 1:
				//Damage Iron Head with either 1 damage or 3 if dashing
				if (gBoss.shock == 0)
				{
					if (flag & 2)
					{
						gBoss.ym = -0x400;
						gMC.ym = 0x400;
						if (gMC.unit == 1)
							DamageBoss(caret_spawner, 3);
						else
							DamageBoss(caret_spawner, 1);
					}
					else if (gMC.unit == 1)
					{
						if (gMC.direct == 0)
							gBoss.xm = -0x400;
						if (gMC.direct == 1)
							gBoss.xm = 0x400;
						DamageBoss(caret_spawner, 3);
					}
				}
				break;
			case 2:
				//Damage Iron Head if hit from below or Ikachan
				if (flag & 2)
				{
					gBoss.ym = -0x400;
					gMC.ym = 0x400;
					if (gMC.unit == 1)
						DamageBoss(caret_spawner, 3);
					else
						DamageBoss(caret_spawner, 1);
				}
				else if (gMC.shock == 0)
				{
					if (gMC.x < gBoss.x + 0x6000)
						gMC.xm = -0x400;
					if (gMC.x > gBoss.x + 0x6000)
						gMC.xm = 0x400;
					DamageMyChar(caret_spawner, 3);
				}
				break;
		}
		
		//Check if Iron Head's been defeated
		if (gBoss.act_no != 0 && gBoss.life == 0)
		{
			event_scr->mode = 1;
			event_scr->x1C = 4;
			event_scr->event_no = gBoss.defeat_event;
			gMC.no_event = 100;
			gBoss.act_no = 0;
			gBoss.shock = 0;
			gBoss.xm = 0;
			gBoss.ym = 0;
		}
	}
}
