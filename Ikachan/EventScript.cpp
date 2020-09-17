#include "EventScript.h"
#include "Draw.h"
#include <stdio.h>

//Rects and other LUTs
RECT rcFade[16] = {
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

int number_tbl[6] = {100000, 10000, 1000, 100, 10, 1};

RECT rcNumber[10] = {
	{  0, 0,  8, 8 },
	{  8, 0, 16, 8 },
	{ 16, 0, 24, 8 },
	{ 24, 0, 32, 8 },
	{ 32, 0, 40, 8 },
	{ 40, 0, 48, 8 },
	{ 48, 0, 56, 8 },
	{ 56, 0, 64, 8 },
	{ 64, 0, 72, 8 },
	{ 72, 0, 80, 8 },
};

RECT grcLine = { 0, 0, 272, 16 };
RECT grcLineClip = { 24, 190, 296, 226};

//Fade dimensions
#define FADE_WIDTH ((SURFACE_WIDTH + 15) / 16)
#define FADE_HEIGHT ((SURFACE_HEIGHT + 15) / 16)

//Fading and other screen effects
BOOL ProcFade(FADE1 *fade)
{
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

//Number drawing
void PutNumber(int x, int y, int no)
{
	BOOL v3 = FALSE;
	int v4;
	for (int i = 0; i < 6; i++)
	{
		v4 = 0;
		while (no >= number_tbl[i])
		{
			++v4;
			no -= number_tbl[i];
			v3 = 1;
		}
		if (v3 || i == 5)
			PutBitmap3(&grcFull, x + (8 * i), y, &rcNumber[v4], SURFACE_ID_FIGURE);
	}
}

void PutNumber2(int x, int y, int no)
{
	
}

//Some debug function I think
void DebugPutText(LPCTSTR text)
{
	PutText(0, 1, text, 0xFFFFFF);
}

//Read event script file
BOOL ReadEventScript(LPCTSTR path, EVENT_SCR *ptx)
{
	//Get filesize
	HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ptx->size = GetFileSize(hFile, NULL);
	CloseHandle(hFile);

	//Allocate data
	ptx->data = (char*)LocalAlloc(LPTR, ptx->size + 1);

	//Open file
	FILE *fp = fopen(path, "rt");
	if (fp == NULL)
		return FALSE;

	//Read file
	fread(ptx->data, ptx->size, 1, fp);
	fclose(fp);
	return TRUE;
}

//Saving
LPCSTR save_magic = "ika_en03";

BOOL SaveRecord()
{
	return TRUE;
}

BOOL LoadRecord()
{
	return TRUE;
}

//Event script functions
BOOL JumpEventScript(EVENT_SCR *ptx)
{
	for (ptx->p_read = 0; ptx->p_read < ptx->size; ptx->p_read++)
	{
		//Check if this is an event marker
		if (ptx->data[ptx->p_read] == '#' && ptx->data[++ptx->p_read] == 'e')
		{
			//Get event number
			++ptx->p_read;
			short no = GetEventScriptNo(ptx);

			//Skip to new-line
			while (ptx->data[ptx->p_read] != '\n')
				ptx->p_read++;
			ptx->p_read++;

			//Check if this is our intended event
			if (no == ptx->event_no)
				return TRUE;
			if (no < ptx->event_no)
				return FALSE;
		}
	}
	return FALSE;
}

short GetEventScriptNo(EVENT_SCR *ptx)
{
	short b = 0;
	b += (ptx->data[ptx->p_read++] - '0') * 1000;
	b += (ptx->data[ptx->p_read++] - '0') * 100;
	b += (ptx->data[ptx->p_read++] - '0') * 10;
	b += ptx->data[ptx->p_read++] - '0';
	return b;
}

void PutEventScriptCursor(EVENT_SCR *ptx)
{
	static RECT rcCursor[] = {
		{ 64,  0, 72, 16 },
		{ 72,  0, 80, 16 },
		{ 56, 16, 64, 32 },
		{ 48, 16, 56, 32 },
		{ 48, 16, 56, 32 },
		{ 56, 16, 64, 32 },
		{ 64, 16, 72, 32 },
		{ 72, 16, 80, 32 },
	};
	PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) + 132, SURFACE_HEIGHT - 26, &rcCursor[(ptx->ani_cursor >> 2) % 8], SURFACE_ID_CURSOR);
}

void PutMsgBox(EVENT_SCR *ptx)
{
	static RECT rcMsgBox = { 0, 0, 300, 48 };

	if (ptx->msg_box == TRUE)
	{
		++ptx->ani_cursor;
		PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 152, SURFACE_HEIGHT - 56, &rcMsgBox, SURFACE_ID_MSGBOX);
		PutBitmap3(&grcLineClip, (SURFACE_WIDTH / 2) - 136, (SURFACE_HEIGHT - 50) + ptx->ypos_line[0], &grcLine, SURFACE_ID_TEXT0);
		PutBitmap3(&grcLineClip, (SURFACE_WIDTH / 2) - 136, (SURFACE_HEIGHT - 50) + ptx->ypos_line[1], &grcLine, SURFACE_ID_TEXT1);
		if (ptx->mode == 5 || ptx->mode == 6 || ptx->mode == 7)
			PutEventScriptCursor(ptx);
	}
}

void InitEventScript(EVENT_SCR *ptx)
{
	ptx->mode = 0;
	ptx->x8 = 0;
	ptx->event_no = 0;
	ptx->ani_cursor = 0;
	ptx->x1A = 0;
	ptx->x1C = 4;
	ptx->line = 0;
	ptx->ypos_line[0] = 0;
	ptx->ypos_line[1] = 20;
	ptx->x21 = 0;
}
