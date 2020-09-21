#include "Effect.h"
#include "System.h"
#include "Draw.h"
#include "Map.h"

//Find free caret slot
int FindCaret(CARET *caret)
{
	//Return index of first free caret or 0xFFFFFF
	for (int i = 0; i < MAX_CARETS; i++, caret++)
	{
		if (caret->cond == FALSE)
			return i;
	}
	return NO_CARET;
}

//Caret process
RECT rcCaret[20] =
{
	{  0, 0,  8,  8 },
	{  8, 0, 16,  8 },
	{ 16, 0, 24,  8 },
	{ 24, 0, 32,  8 },
	{ 32, 0, 40,  8 },
	{ 40, 0, 48,  8 },
	{ 48, 0, 56,  8 },
	{ 56, 0, 64,  8 },
	{ 64, 0, 72,  8 },
	{ 72, 0, 80,  8 },
	{  0, 8,  8, 16 },
	{  8, 8, 16, 16 },
	{ 16, 8, 24, 16 },
	{ 24, 8, 32, 16 },
	{ 32, 8, 40, 16 },
	{ 40, 8, 48, 16 },
	{ 48, 8, 56, 16 },
	{ 56, 8, 64, 16 },
	{ 64, 8, 72, 16 },
	{ 72, 8, 80, 16 }
};

void ProcCaret00(CARET *caret, MAP *map, FRAME *frame)
{
	//Fall and move
	caret->ym += 32;
	caret->x += caret->xm;
	caret->y += caret->ym;
	
	//Delete if out of bounds
	if (caret->x > (map->width << 14) || caret->x < 0 || caret->y > (map->length << 14) || caret->y < 0)
		caret->cond = FALSE;
	
	//Animate
	if (++caret->ani_wait > 8)
	{
		caret->ani_wait = 0;
		if (++caret->ani_no > 5)
			caret->cond = FALSE;
	}
	
	//Draw
	if (caret->cond)
		PutBitmap3(&grcFull,
		           (caret->x - frame->x) / 0x400 - 4,
		           (caret->y - frame->y) / 0x400 - 4,
		           &rcCaret[caret->ani_no],
		           SURFACE_ID_STAR + caret->type);
}

void ProcCaret01(CARET *caret, MAP *map, FRAME *frame)
{
	//Float upwards
	caret->ym -= 16;
	caret->x += caret->xm;
	caret->y += caret->ym;
	
	//Delete if out of bounds
	if (caret->x > (map->width << 14) || caret->x < 0 || caret->y > (map->length << 14) || caret->y < 0)
		caret->cond = FALSE;
	
	//Animate
	if (++caret->ani_wait > 32)
	{
		caret->ani_wait = 0;
		if (++caret->ani_no > 5)
			caret->cond = FALSE;
	}
	
	//Draw
	if (caret->cond)
		PutBitmap3(&grcFull,
		           (caret->x - frame->x) / 0x400 - 4,
		           (caret->y - frame->y) / 0x400 - 4,
		           &rcCaret[caret->ani_no],
		           SURFACE_ID_STAR + caret->type);
}

void ProcCaret02(CARET *caret, MAP *map, FRAME *frame)
{
	//Move up
	caret->y -= 342;
	
	//Delete if out of bounds
	if (caret->x > (map->width << 14) || caret->x < 0 || caret->y > (map->length << 14) || caret->y < 0)
		caret->cond = FALSE;
	
	//Delete after 72 frames
	if (++caret->ani_wait > 72)
		caret->cond = FALSE;
	
	//Draw
	if (caret->cond)
		PutBitmap3(&grcFull,
		           (caret->x - frame->x) / 0x400 - 4,
		           (caret->y - frame->y) / 0x400 - 4,
		           &rcCaret[caret->ani_no],
		           SURFACE_ID_STAR + caret->type);
}

void ProcCaret03(CARET *caret, MAP *map, FRAME *frame)
{
	RECT rcLevelUp[2] = {
		{ 0,  0, 64, 16 },
		{ 0, 16, 64, 32 },
	};
	
	//Move up
	caret->y -= 342;
	
	//Delete if out of bounds
	if (caret->x > (map->width << 14) || caret->x < 0 || caret->y > (map->length << 14) || caret->y < 0)
		caret->cond = FALSE;
	
	//Animate
	if (++caret->ani_wait > 3)
	{
		caret->ani_wait = 0;
		if (++caret->ani_no > 24)
			caret->cond = FALSE;
	}
	
	//Draw
	if (caret->cond)
		PutBitmap3(&grcFull,
		           (caret->x - frame->x) / 0x400 - 32,
		           (caret->y - frame->y) / 0x400 - 4,
		           &rcLevelUp[caret->ani_no % 2],
		           SURFACE_ID_STAR + caret->type);
}

typedef void (*CARETPROC)(CARET*, MAP*, FRAME*);
CARETPROC gpCaretProcTbl[] = {
	ProcCaret00,
	ProcCaret01,
	ProcCaret02,
	ProcCaret03,
};

void ProcCaret(CARET *caret, MAP *map, FRAME *frame)
{
	for (int i = 0; i < MAX_CARETS; i++, caret++)
		if (caret->cond)
			gpCaretProcTbl[caret->type](caret, map, frame);
}

int FindCaretSpawner(CARET_SPAWNER *caret_spawner)
{
	//Return index of first free caret spawner or 0xFFFFFF
	for (int i = 0; i < MAX_CARETS; i++, caret_spawner++) //This should be MAX_CARET_SPAWNERS
	{
		if (caret_spawner->cond == FALSE)
			return i;
	}
	return NO_CARET;
}

void ProcCaretSpawner(CARET_SPAWNER *caret_spawner, CARET *caret)
{
	for (int i = 0; i < MAX_CARET_SPAWNERS; i++, caret_spawner++)
	{
		if (caret_spawner->cond)
		{
			caret_spawner->cond = FALSE;
			for (int j = 0; j < caret_spawner->num; j++)
			{
				int caret_i = FindCaret(caret);
				if (caret_i != NO_CARET)
				{
					CARET *set_caret = &caret[caret_i];
					set_caret->type = caret_spawner->type;
					set_caret->ani_wait = 0;
					set_caret->ani_no = caret_spawner->ani_no;
					set_caret->x = caret_spawner->x + (Random(-caret_spawner->rand_x, caret_spawner->rand_x) << 10);
					set_caret->y = caret_spawner->y + (Random(-caret_spawner->rand_y, caret_spawner->rand_y) << 10);
					set_caret->xm = Random(caret_spawner->rand_moveleft, caret_spawner->rand_moveright);
					set_caret->ym = Random(caret_spawner->rand_moveup, caret_spawner->rand_movedown);
					set_caret->cond = TRUE;
				}
			}
		}
	}
}
