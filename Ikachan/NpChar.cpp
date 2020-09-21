#include "NpChar.h"
#include "Draw.h"
#include "System.h"
#include "EventScript.h"
#include "Map.h"
#include "Player.h"
#include "Sound.h"
#include <stdio.h>

//NPC initialization
void InitNpChar(NPCHAR *npc)
{
	for (int i = 0; i < MAX_NPCS; i++)
	{
		npc[i].cond = FALSE;
		npc[i].type = 0;
		npc[i].code_char = 0;
		npc[i].code_event = 0;
		npc[i].act_wait = 0;
		npc[i].act_no = 0;
		npc[i].ani_no = 0;
		npc[i].ani_wait = 0;
		npc[i].direct = 0;
		npc[i].x = 0;
		npc[i].y = 0;
		npc[i].xm = 0;
		npc[i].ym = 0;
		npc[i].tgt_x = 0;
		npc[i].tgt_y = 0;
	}
}

//NPC drawing
void PutNpChar00(NPCHAR *npc, FRAME *frame)
{
	static RECT rcNpc00[8] = {
		{  0,  0, 16, 16 },
		{ 16,  0, 32, 16 },
		{ 32,  0, 48, 16 },
		{ 48,  0, 64, 16 },
		{  0, 16, 16, 32 },
		{ 16, 16, 32, 32 },
		{ 32, 16, 48, 32 },
		{ 48, 16, 64, 32 },
	};
	
	if (npc->cond != FALSE)
	{
		PutBitmap3(&grcFull,
		           (npc->x / 0x400) - (frame->x / 0x400),
		           (npc->y / 0x400) - (frame->y / 0x400),
		           &rcNpc00[(npc->direct * 4) + npc->ani_no],
		           SURFACE_ID_HARI + npc->code_char);
	}
}

void PutNpChar01(NPCHAR *npc, FRAME *frame)
{
	static RECT rcNpc01[4] = {
		{ 0,  0, 16, 16 },
		{ 0, 16, 16, 32 },
		{ 0, 32, 16, 48 },
		{ 0, 48, 16, 64 },
	};
	
	if (npc->cond != FALSE)
	{
		//Get framerect
		RECT *rc = &rcNpc01[(npc->direct * 2) + npc->ani_no];
		RECT rect;
		rect.left = rc->left;
		rect.top = rc->top;
		rect.right = rc->right;
		rect.bottom = rc->bottom;
		rect.left += 16 * ((npc->tgt_x - npc->x) / 0x4000);
		rect.right = rect.left + 16;
		
		//Draw
		PutBitmap3(&grcFull,
		           (npc->x / 0x400) - (frame->x / 0x400),
		           (npc->y / 0x400) - (frame->y / 0x400),
		           &rect,
		           SURFACE_ID_HARI + npc->code_char);
	}
}

void PutNpChar02(NPCHAR *npc, FRAME *frame)
{
	static RECT rcNpc02[8] = {
		{  0,  0, 16, 16 },
		{ 16,  0, 32, 16 },
		{  0, 16, 16, 32 },
		{ 16, 16, 32, 32 },
		{  0,  0, 16, 16 },
		{ 16,  0, 32, 16 },
		{  0, 16, 16, 32 },
		{ 16, 16, 32, 32 },
	};
	
	if (npc->cond != FALSE)
	{
		PutBitmap3(&grcFull,
		           (npc->x / 0x400) - (frame->x / 0x400),
		           (npc->y / 0x400) - (frame->y / 0x400),
		           &rcNpc02[(npc->direct * 4) + npc->ani_no],
		           SURFACE_ID_HARI + npc->code_char);
	}
}

void PutNpChar03(NPCHAR *npc, FRAME *frame)
{
	static RECT rcNpc03[4] = {
		{  0,  0, 20, 20 },
		{ 20,  0, 40, 20 },
		{  0, 20, 20, 40 },
		{ 20, 20, 40, 40 },
	};
	
	if (npc->cond != FALSE)
	{
		PutBitmap3(&grcFull,
		           (npc->x / 0x400) - (frame->x / 0x400) - 2,
		           (npc->y / 0x400) - (frame->y / 0x400) - 4,
		           &rcNpc03[npc->ani_no],
		           SURFACE_ID_HARI + npc->code_char);
	}
}

void PutNpChar04(NPCHAR *npc, FRAME *frame)
{
	static RECT rcNpc04[6] =
	{
		{  0, 0,  8,  8 },
		{  0, 8,  8, 16 },
		{  8, 0, 16,  8 },
		{  8, 8, 16, 16 },
		{ 16, 0, 24,  8 },
		{ 16, 8, 24, 16 },
	};
	
	if (npc->cond != FALSE)
	{
		PutBitmap3(&grcFull,
		           (npc->x / 0x400) - (frame->x / 0x400) + 4,
		           (npc->y / 0x400) - (frame->y / 0x400) + 4,
		           &rcNpc04[(npc->direct * 2) + npc->ani_no],
		           SURFACE_ID_HARI + npc->code_char);
	}
}

void PutNpChar05(NPCHAR *npc, FRAME *frame)
{
	static RECT rcNpc05[6] =
	{
		{  0,  0, 16, 16 },
		{  0, 16, 16, 32 },
		{ 16,  0, 32, 16 },
		{ 16, 16, 32, 32 },
		{ 32,  0, 48, 16 },
		{ 32, 16, 48, 32 },
	};
	
	if (npc->cond != FALSE)
	{
		PutBitmap3(&grcFull,
		           (npc->x / 0x400) - (frame->x / 0x400),
		           (npc->y / 0x400) - (frame->y / 0x400),
		           &rcNpc05[(npc->direct * 2) + npc->ani_no],
		           SURFACE_ID_HARI + npc->code_char);
	}
}

void PutNpChar06(NPCHAR *npc, FRAME *frame)
{
	static RECT rcNpc06[4] = {
		{  0,  0, 30, 20 },
		{  0, 20, 30, 40 },
		{ 30,  0, 60, 20 },
		{ 30, 20, 60, 40 },
	};
	
	if (npc->cond != FALSE)
	{
		PutBitmap3(&grcFull,
		           (npc->x / 0x400) - (frame->x / 0x400) - 6,
		           (npc->y / 0x400) - (frame->y / 0x400) - 2,
		           &rcNpc06[(npc->direct * 2) + npc->ani_no],
		           SURFACE_ID_HARI + npc->code_char);
	}
}

void PutNpChar07(NPCHAR *npc, FRAME *frame)
{
	static RECT rcNpc07 = { 0, 0, 32, 32 };
	
	if (npc->cond != FALSE)
	{
		PutBitmap3(&grcFull,
		           (npc->x / 0x400) - (frame->x / 0x400) - 8,
		           (npc->y / 0x400) - (frame->y / 0x400) - 16,
		           &rcNpc07,
		           SURFACE_ID_HARI + npc->code_char);
	}
}

typedef void (*NPCPUT)(NPCHAR*, FRAME*);
NPCPUT gpNpcPutTbl[] = {
	PutNpChar00,
	PutNpChar01,
	PutNpChar02,
	PutNpChar03,
	PutNpChar04,
	PutNpChar05,
	PutNpChar01,
	PutNpChar06,
	PutNpChar00,
	PutNpChar07,
};

void PutNpChar(NPCHAR *npc, FRAME *frame)
{
	for (int i = 0; i < MAX_NPCS; i++, npc++)
		if (npc->cond != FALSE)
			gpNpcPutTbl[npc->code_char](npc, frame);
}

//Update
void ActNpChar00(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			//Move towards target
			if (npc->x > npc->tgt_x)
				npc->xm -= 6;
			if (npc->x < npc->tgt_x)
				npc->xm += 6;
			if (npc->y > npc->tgt_y)
				npc->ym -= 4;
			if (npc->y < npc->tgt_y)
				npc->ym += 4;
			
			//Face in moving direction
			if (npc->xm < 0)
				npc->direct = 0;
			if (npc->xm > 0)
				npc->direct = 1;
			
			//Animate
			if (++npc->ani_wait > 60)
			{
				npc->ani_wait = 0;
				if (++npc->ani_no > 1)
					npc->ani_no = 0;
			}
			
			//Limit speed
			if (npc->xm > 0x400)
				npc->xm = 0x400;
			if (npc->xm < -0x400)
				npc->xm = -0x400;
			if (npc->ym > 0x400)
				npc->ym = 0x400;
			if (npc->ym < -0x400)
				npc->ym = -0x400;
			
			//Move
			npc->x += npc->xm;
			npc->y += npc->ym;
			
			//Puff up if Ikachan is nearby (and doesn't have a pearl)
			if (npc->act_wait > 0)
				--npc->act_wait;
			
			if (npc->act_wait == 0 && (gMC.equip & 4) == 0 &&
				(npc->x - 0xA000) < gMC.x &&
				(npc->x + 0xA000) > gMC.x &&
				(npc->y - 0xA000) < gMC.y &&
				(npc->y + 0xA000) > gMC.y)
			{
				if (npc->type == 2)
					npc->act_no = 1;
				npc->act_wait = 300;
			}
			break;
		case 1:
			//Face towards Ikachan
			if (npc->x > gMC.x)
				npc->direct = 0;
			if (npc->x < gMC.x)
				npc->direct = 1;
			
			//Animate
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				if (++npc->ani_no > 3)
					npc->ani_no = 2;
			}
			
			//Stop puffing up after 100 frames
			if (--npc->act_wait <= 200)
			{
				npc->act_no = 0;
				npc->ani_no = 0;
			}
			break;
	}
}

void ActNpChar01(NPCHAR *npc)
{
	//Increment animation timer
	npc->ani_wait++;
	
	//Fall and move
	if (npc->ym < 0x800)
		npc->ym += 20;
	npc->y += npc->ym;
	
	//Face towards Ikachan
	if (npc->x < gMC.x)
		npc->direct = 1;
	if (npc->x > gMC.x)
		npc->direct = 0;
	
	//Animate
	if (npc->ani_wait > 40)
	{
		npc->ani_wait = 0;
		if (++npc->ani_no > 1)
			npc->ani_no = 0;
	}
}

void ActNpChar02(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			//Turn around if too far from home
			if (npc->x < npc->tgt_x - 0x10000)
				npc->direct = 1;
			if (npc->x > npc->tgt_x + 0x10000)
				npc->direct = 0;
			
			//Move in facing direction
			if (npc->direct == 0 && npc->xm > -0x200 )
				npc->xm -= 16;
			if (npc->direct == 1 && npc->xm < 0x200 )
				npc->xm += 16;
			
			//Jump after we've been on the ground for 80 frames
			if (npc->airborne == FALSE)
				npc->act_wait++;
			
			if (npc->act_wait > 80)
			{
				npc->act_wait = 0;
				if (npc->type != 2 || (gMC.equip & 4))
				{
					//Jump
					npc->ym = -1024;
				}
				else
				{
					//Attacking jump
					npc->ym = -1536;
					npc->act_no = 1;
				}
			}
			
			//Animate
			if (++npc->ani_wait > 10)
			{
				npc->ani_wait = 0;
				if (++npc->ani_no > 1)
					npc->ani_no = 0;
			}
			
			//Fall and move
			if (npc->ym < 0x800)
				npc->ym += 20;
			npc->x += npc->xm;
			npc->y += npc->ym;
			
			//Do falling animation if airborne
			if (npc->airborne == TRUE)
				npc->ani_no = 2;
			break;
		case 1:
			//Animate
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				if (++npc->ani_no > 3)
					npc->ani_no = 2;
			}
			
			//Turn around if too far from home
			if (npc->x < npc->tgt_x - 0x10000)
				npc->direct = 1;
			if (npc->x > npc->tgt_x + 0x10000)
				npc->direct = 0;
			
			//Move in facing direction
			if (npc->direct == 0 && npc->xm > -0x200 )
				npc->xm -= 16;
			if (npc->direct == 1 && npc->xm < 0x200 )
				npc->xm += 16;
			
			//Fall and move
			if (npc->ym < 0x800)
				npc->ym += 20;
			npc->x += npc->xm;
			npc->y += npc->ym;
			
			//Stop attacking once moving down
			if (npc->ym > 0)
				npc->act_no = 0;
			break;
	}
}

void ActNpChar03(NPCHAR *npc)
{
	//Fall and move
	if (npc->ym < 0x800)
		npc->ym += 20;
	npc->y += npc->ym;
	
	//Animate
	if (npc->act_no == 0)
	{
		if (npc->act_wait > 0)
			npc->act_wait--;
		if ((npc->x - 0x8000) >= gMC.x || (npc->x + 0x8000) <= gMC.x || (npc->y - 0x8000) >= gMC.y || npc->y <= gMC.y)
		{
			if (++npc->ani_wait > 5)
			{
				npc->ani_wait = 0;
				if (--npc->ani_no < 0)
					npc->ani_no = 0;
			}
		}
		else if (++npc->ani_wait > 2)
		{
			npc->ani_wait = 0;
			if (++npc->ani_no > 3)
				npc->ani_no = 3;
		}
	}
}

void ActNpChar04(NPCHAR *npc)
{
	//Move towards Ikachan
	if (npc->x > gMC.x)
		npc->xm -= 24;
	if (npc->x < gMC.x)
		npc->xm += 24;
	if (npc->y > gMC.y)
		npc->ym -= 16;
	if (npc->y < gMC.y)
		npc->ym += 16;
	
	//Face towards Ikachan
	if (npc->x > gMC.x)
		npc->direct = 0;
	if (npc->x < gMC.x)
		npc->direct = 1;
	if ((npc->x - 0x8000) < gMC.x && (npc->x + 0x8000) > gMC.x && (npc->y - 0x8000) < gMC.y && (npc->y + 0x8000) > gMC.y)
		npc->direct = 2;
	npc->act_no = (npc->y + 0x2000) > gMC.y;
	
	//Animate
	if (++npc->ani_wait > 8)
	{
		npc->ani_wait = 0;
		if (++npc->ani_no > 1)
			npc->ani_no = 0;
	}
	
	//Limit speed
	if (npc->xm > 0x400)
		npc->xm = 0x400;
	if (npc->xm < -0x400)
		npc->xm = -0x400;
	if (npc->ym > 0x400)
		npc->ym = 0x400;
	if (npc->ym < -0x400)
		npc->ym = -0x400;
	
	//Move
	npc->x += npc->xm;
	npc->y += npc->ym;
}

void ActNpChar05(NPCHAR *npc)
{
	//Fall and move
	if (npc->ym < 0x800)
		npc->ym += 20;
	npc->y += npc->ym;
}

void ActNpChar06(NPCHAR *npc)
{
	if (npc->act_no == 0)
	{
		//Move towards target
		if (npc->x > npc->tgt_x)
			npc->xm -= 12;
		if (npc->x < npc->tgt_x)
			npc->xm += 12;
		if (npc->y > npc->tgt_y)
			npc->ym -= 8;
		if (npc->y < npc->tgt_y)
			npc->ym += 8;
		
		//Face in moving direction
		if (npc->xm < 0)
			npc->direct = 0;
		if (npc->xm > 0)
			npc->direct = 1;
		
		//Animate
		if (++npc->ani_wait > 30)
		{
			npc->ani_wait = 0;
			if (++npc->ani_no > 1)
				npc->ani_no = 0;
		}
		
		//Limit speed
		if (npc->xm > 0x800)
			npc->xm = 0x800;
		if (npc->xm < -0x800)
			npc->xm = -0x800;
		if (npc->ym > 0x800)
			npc->ym = 0x800;
		if (npc->ym < -0x800)
			npc->ym = -0x800;
		
		//Move
		npc->x += npc->xm;
		npc->y += npc->ym;
	}
}

typedef void (*NPCACT)(NPCHAR*);
NPCACT gpNpcActTbl[] = {
	ActNpChar00,
	ActNpChar01,
	ActNpChar02,
	ActNpChar03,
	ActNpChar04,
	ActNpChar04,
	ActNpChar05,
	ActNpChar01,
	ActNpChar06,
	ActNpChar05,
};

void ActNpChar(NPCHAR *npc)
{
	for (int i = 0; i < MAX_NPCS; i++, npc++)
		if (npc->cond != FALSE)
			gpNpcActTbl[npc->code_char](npc);
}

//NPC collision
BYTE JudgeHitNpCharBlock(NPCHAR *npc, int x, int y, BYTE flag)
{
	if ((flag & 1) && (flag & 2))
	{
		if ((npc->x / 0x400) < (x * 16 + 15) && (npc->y / 0x400) < (y * 16 + 12))
		{
			npc->x = (x * 16 + 15) << 10;
			npc->xm = 0;
			npc->flag |= 1;
		}
		if ((npc->y / 0x400) < (y * 16 + 15) && (npc->x / 0x400) < (x * 16 + 12))
		{
			npc->y = (y * 16 + 16) << 10;
			npc->ym = 0;
			npc->flag |= 2;
		}
	}
	
	if ((flag & 4) && (flag & 2))
	{
		if (((npc->x + 0x3FF) / 0x400) > (x * 16 - 14) && (npc->y / 0x400) < (y * 16 + 12))
		{
			npc->x = (x * 16 - 14) << 10;
			npc->xm = 0;
			npc->flag |= 4;
		}
		if ((npc->y / 0x400) < (y * 16 + 15) && (npc->x / 0x400) > (x * 16 - 12))
		{
			npc->y = (y * 16 + 16) << 10;
			npc->ym = 0;
			npc->flag |= 2;
		}
	}
	
	if ((flag & 1) && (flag & 8))
	{
		if ((npc->x / 0x400) < (x * 16 + 15) && (npc->y / 0x400) > (y * 16 - 12))
		{
			npc->x = (x * 16 + 15) << 10;
			npc->xm = 0;
			npc->flag |= 1;
		}
		if ((npc->y / 0x400) >= (y * 16 - 16) && (npc->x / 0x400) < (x * 16 + 12))
		{
			npc->airborne = FALSE;
			npc->y = (y * 16 - 16) << 10;
			if (npc->ym > 0)
				npc->ym = 0;
			npc->flag |= 8;
		}
	}
	
	if ((flag & 4) && (flag & 8))
	{
		if (((npc->x + 0x3FF) / 0x400) > (x * 16 - 14) && (npc->y / 0x400) > (y * 16 - 12))
		{
			npc->x = (x * 16 - 14) << 10;
			npc->xm = 0;
			npc->flag |= 4;
		}
		if ((npc->y / 0x400) >= (y * 16 - 16) && (npc->x / 0x400) > (x * 16 - 12))
		{
			npc->airborne = FALSE;
			npc->y = (y * 16 - 16) << 10;
			if (npc->ym > 0)
				npc->ym = 0;
			npc->flag |= 8;
		}
	}
	
	return npc->flag;
}

void HitNpCharMap(NPCHAR *npc, MAP *map)
{
	//Collision offsets and flags
	signed char offx[4];
	signed char offy[4];
	BOOLEAN flag1[4];
	BYTE flag2[4];
	
	offx[0] = 0;
	offx[1] = 1;
	offx[2] = 0;
	offx[3] = 1;
	
	offy[0] = 0;
	offy[1] = 0;
	offy[2] = 1;
	offy[3] = 1;
	
	flag1[0] = 0;
	flag1[1] = 0;
	flag1[2] = 1;
	flag1[3] = 1;
	
	flag2[0] = 1 | 2;
	flag2[1] = 4 | 2;
	flag2[2] = 8 | 1;
	flag2[3] = 8 | 4;
	
	for (int i = 0; i < MAX_NPCS; i++, npc++)
	{
		if (npc->cond != FALSE)
		{
			//Get collision position and reset state
			int x = npc->x / 0x400 / 16;
			int y = npc->y / 0x400 / 16;
			signed char v4 = 2;
			npc->flag = 0;
			
			for (int j = 0; j < 4; j++)
			{
				//Get tile at the position to check
				int part = map->data[(x + offx[j]) + map->width * (y + offy[j])];
				
				//Block collision
				if ((part < 0x80 || part >= 0xA0) && (part < 0xE0 || part >= 0x100))
					v4 -= flag1[j];
				else if ((JudgeHitNpCharBlock(npc, x + offx[j], y + offy[j], flag2[j]) & 8) == 0)
					v4 -= flag1[j];
			}
			
			//Set airborne flag
			if (v4 <= 0)
				npc->airborne = TRUE;
		}
	}
}

void HitMyCharNpChar(NPCHAR *npc, EVENT_SCR *event_scr, CARET_SPAWNER *caret_spawner)
{
	for (int i = 0; i < MAX_NPCS; i++, npc++)
	{
		BOOL touch = FALSE;
		if (npc->cond == FALSE)
			continue;
		
		//Check for non-solid contact
		if (npc->type != 3 && (npc->type != 2 || gMC.shock != 0))
		{
			if (npc->type == 0 &&
				gMC.no_event == 0 &&
				gMC.x < npc->x + 0x1000 &&
				gMC.x > npc->x - 0x1000 &&
				gMC.y < npc->y + 0x1000 &&
				gMC.y > npc->y - 0x1000)
			{
				//Start NPC's event
				event_scr->mode = 1;
				event_scr->x1C = 4;
				event_scr->event_no = npc->code_event;
				gMC.no_event = 100;
				continue;
			}
		}
		else
		{
			//Solid contact
			if (gMC.x < (npc->x + 0x3400) && gMC.x > (npc->x + 0x2000) && gMC.y < (npc->y + 0x3000) && gMC.y > (npc->y - 0x3000))
			{
				gMC.x = npc->x + 0x3400;
				gMC.xm = 0;
				gMC.flag |= 1;
				touch = TRUE;
			}
			if (gMC.y < (npc->y + 0x3400) && gMC.y > (npc->y + 0x2000) && gMC.x < (npc->x + 0x3000) && gMC.x > (npc->x - 0x3000))
			{
				if (gMC.ym < -100)
					PlaySoundObject(SOUND_ID_HITHEAD, SOUND_MODE_PLAY);
				gMC.y = npc->y + 0x3400;
				gMC.ym = 0;
				gMC.flag |= 2;
				touch = TRUE;
			}
			if (gMC.x > (npc->x - 0x3400) && (gMC.x + 0x3FF) < (npc->x - 0x2000) && gMC.y < (npc->y + 0x3000) && gMC.y > (npc->y - 0x3000))
			{
				gMC.x = npc->x - 0x3400;
				gMC.xm = 0;
				gMC.flag |= 4;
				touch = TRUE;
			}
			if (gMC.y >= (npc->y - 0x3400) && gMC.y < (npc->y - 0x2000) && gMC.x > (npc->x - 0x3000) && gMC.x < (npc->x + 0x3000))
			{
				gMC.airborne = FALSE;
				gMC.y = npc->y - 0x3400;
				if (gMC.ym > 0)
					gMC.ym = 0;
				gMC.flag |= 8;
				touch = TRUE;
			}
		}
		
		if (touch && gMC.no_event == 0)
		{
			static BYTE npc_damage[] = { 1, 0, 2, 4, 4, 2, 0, 0 };
			static BYTE npc_defense[] = { 1, 4, 2, 9, 3, 2, 0, 0 };
			static BYTE npc_exp[] = {1, 0, 3, 0, 0, 3, 0, 0 };
			
			switch (npc->type)
			{
				case 2:
					//Hurt Ikachan
					if (npc->act_no == 1)
					{
						if (gMC.x < npc->x)
							gMC.xm = -0x400;
						if (gMC.x > npc->x)
							gMC.xm = 0x400;
						DamageMyChar(caret_spawner, npc_damage[npc->code_char]);
					}
					
					//Check if we should hurt the NPC
					if (gMC.flag != 0 && gMC.unit == 1 && gMC.flag != 8 || (gMC.flag & 2) && (gMC.equip & 1))
					{
						if (npc_defense[npc->code_char] > gMC.level)
						{
							//The NPC was too strong
							if ( !gMC.no_event )
								PlaySoundObject(SOUND_ID_NODMG, SOUND_MODE_PLAY);
							gMC.no_event = 100;
						}
						else
						{
							//Award us experience
							PlaySoundObject(SOUND_ID_WIN, SOUND_MODE_PLAY);
							gMC.exp += npc_exp[npc->code_char];
							int exp_i = FindCaretSpawner(caret_spawner);
							if (exp_i != 0xFFFFFF )
							{
								CARET_SPAWNER *caretsp = &caret_spawner[exp_i];
								caretsp->cond = TRUE;
								caretsp->type = 2;
								caretsp->ani_no = npc_exp[npc->code_char] + 10;
								caretsp->num = 1;
								caretsp->x = npc->x + 0x2000;
								caretsp->y = npc->y - 0x1000;
								caretsp->rand_x = 1;
								caretsp->rand_y = 0;
							}
							
							//Destroy the NPC
							npc->cond = FALSE;
							int dead_i = FindCaretSpawner(caret_spawner);
							if (dead_i != 0xFFFFFF )
							{
								CARET_SPAWNER *caretsp = &caret_spawner[dead_i];
								caretsp->cond = TRUE;
								caretsp->type = 0;
								caretsp->ani_no = 0;
								caretsp->num = 6;
								caretsp->x = npc->x + 0x2000;
								caretsp->y = npc->y + 0x2000;
								caretsp->rand_moveright = 0x800;
								caretsp->rand_moveleft = -0x800;
								caretsp->rand_movedown = -0x200;
								caretsp->rand_moveup = -0x800;
								caretsp->rand_x = 8;
								caretsp->rand_y = 8;
							}
							
							//Start NPC's event
							event_scr->mode = 1;
							event_scr->x1C = 4;
							event_scr->event_no = npc->code_event;
							gMC.no_event = 100;
						}
					}
					break;
				case 3:
					//Start NPC's event
					event_scr->mode = 1;
					event_scr->x1C = 4;
					event_scr->event_no = npc->code_event;
					gMC.no_event = 100;
					break;
			}
		}
	}
}

//Reload NPCs
BOOL ReloadNpChar(NPCHAR *npc)
{
	//Open NPChar.dat
	TCHAR path[MAX_PATH];
	sprintf(path, "%s\\%s", gModulePath, "NPChar.dat");
	
	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
		return FALSE;
	
	//Read NPC data
	for (int i = 0; i < MAX_NPCS; i++)
	{
		//Read individual NPC
		NPCHAR event;
		fread(&event.cond, 1, 1, fp);
		fread(&event.type, 1, 1, fp);
		fread(&event.code_char, 1, 1, fp);
		fread(&event.code_event, 2, 1, fp);
		fread(&event.direct, 1, 1, fp);
		fread(&event.x, 4, 1, fp);
		fread(&event.y, 4, 1, fp);
		fread(&event.tgt_x, 4, 1, fp);
		fread(&event.tgt_y, 4, 1, fp);
		
		//Zero data that isn't read
		event.act_wait = 0;
		event.act_no = 0;
		event.ani_no = 0;
		event.ani_wait = 0;
		event.xm = 0;
		event.ym = 0;
		
		//Copy NPC only if some value is 2
		if (event.type == 2)
			memcpy(&npc[i], &event, sizeof(NPCHAR));
	}
	return TRUE;
}
