#include "EventScript.h"
#include "Draw.h"
#include <stdio.h>

const RECT grcLine = { 0, 0, 272, 16 };
const RECT grcLineClip = { 24, 190, 296, 226};

//Some debug function I think
void DebugPutText(LPCTSTR text)
{
	PutText(0, 1, text, 0xFFFFFF);
}

//Read an event script file
BOOL ReadEventScript(LPCTSTR path, EVENT_SCR *ptx)
{
	//Get filesize
	HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ptx->size = GetFileSize(hFile, NULL);
	CloseHandle(hFile);

	//Allocate data
	ptx->data = (CHAR*)LocalAlloc(LPTR, ptx->size + 1);

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
