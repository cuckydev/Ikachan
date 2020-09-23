#include "Editor.h"
#include "System.h"
#include "Draw.h"
#include "EventScript.h"
#include "Sound.h"
#include <stdio.h>

BOOLEAN gEditorMode;
short gEditorNPC;
POINT gEditorCursor_Track;
POINT gEditorCursor_WorldPos;
POINT gEditorCursor_ScreenPos;
POINT gEditorCursor_Tile;
POINT gEditorCursor_NPC;

void InitEditor()
{
	gEditorCursor_Track.x = (SURFACE_WIDTH / 2) - 16;
	gEditorCursor_Track.y = (SURFACE_HEIGHT / 2) - 16;
	SetCursorPos((SURFACE_WIDTH / 2) - 16, (SURFACE_HEIGHT / 2) - 16);
	gEditorCursor_ScreenPos.x = (SURFACE_WIDTH / 2) - 16;
	gEditorCursor_ScreenPos.y = (SURFACE_HEIGHT / 2) - 16;
}

void PutEditorCursor()
{
	//Get cursor's position
	POINT cur_pos;
	GetCursorPos(&cur_pos);
	
	//Move cursor
	gEditorCursor_ScreenPos.x += cur_pos.x - gEditorCursor_Track.x;
	gEditorCursor_ScreenPos.y += cur_pos.y - gEditorCursor_Track.y;
	gEditorCursor_Track.x = cur_pos.x;
	gEditorCursor_Track.y = cur_pos.y;
	
	//Draw cursor
	static RECT rcCursor = { 0, 0, 32, 32 };
	PutBitmap3(&grcFull, gEditorCursor_ScreenPos.x, gEditorCursor_ScreenPos.y, &rcCursor, SURFACE_ID_CURSOR);
}

void PutEditorSelect(NPCHAR *npc, FRAME *frame)
{
	//Get cursor positions
	gEditorCursor_WorldPos.x = gEditorCursor_ScreenPos.x + (frame->x / 0x400);
	gEditorCursor_WorldPos.y = gEditorCursor_ScreenPos.y + (frame->y / 0x400);
	gEditorCursor_Tile.x = gEditorCursor_WorldPos.x / 16;
	gEditorCursor_Tile.y = gEditorCursor_WorldPos.y / 16;
	
	//Draw selected tile box and coordinate
	static RECT rcTileSelect = { 32, 0, 48, 16 };
	PutBitmap3(&grcFull,
	           ((gEditorCursor_WorldPos.x / 16) * 16) - (frame->x / 0x400),
	           ((gEditorCursor_WorldPos.y / 16) * 16) - (frame->y / 0x400),
	           &rcTileSelect,
	           SURFACE_ID_CURSOR);
	PutNumber2((gEditorCursor_Tile.x * 16) - (frame->x / 0x400) - 24,
	           (gEditorCursor_Tile.y * 16) - (frame->y / 0x400),
	           gEditorCursor_Tile.x);
	PutNumber2((gEditorCursor_Tile.x * 16) - (frame->x / 0x400) - 24,
	           (gEditorCursor_Tile.y * 16) - (frame->y / 0x400) + 8,
	           gEditorCursor_Tile.y);
	
	//Draw selected NPC information
	if (npc[gEditorNPC].cond)
	{
		//Draw NPC select box and coordinate
		static RECT rcNPCSelect = { 32, 16, 48, 32 };
		PutBitmap3(&grcFull,
		           (gEditorCursor_NPC.x * 16) - (frame->x / 0x400),
		           (gEditorCursor_NPC.y * 16) - (frame->y / 0x400),
		           &rcNPCSelect,
		           SURFACE_ID_CURSOR);
		PutNumber2((gEditorCursor_NPC.x * 16) - (frame->x / 0x400) - 24,
		           (gEditorCursor_NPC.y * 16) - (frame->y / 0x400),
		           gEditorCursor_NPC.x);
		PutNumber2((gEditorCursor_NPC.x * 16) - (frame->x / 0x400) - 24,
		           (gEditorCursor_NPC.y * 16) - (frame->y / 0x400) + 8,
		           gEditorCursor_NPC.y);
		
		//Draw NPC target position select box and coordinate
		static RECT rcTgtSelect = { 48, 0, 64, 16 };
		PutBitmap3(&grcFull,
		           (npc[gEditorNPC].tgt_x / 0x400) - (frame->x / 0x400),
		           (npc[gEditorNPC].tgt_y / 0x400) - (frame->y / 0x400),
		           &rcTgtSelect,
		           SURFACE_ID_CURSOR);
		PutNumber2((npc[gEditorNPC].tgt_x / 0x400) - (frame->x / 0x400) - 8,
		           (npc[gEditorNPC].tgt_y / 0x400) - (frame->y / 0x400) - 16,
		           (npc[gEditorNPC].tgt_x / 0x400) / 16);
		PutNumber2((npc[gEditorNPC].tgt_x / 0x400) - (frame->x / 0x400) - 8,
		           (npc[gEditorNPC].tgt_y / 0x400) - (frame->y / 0x400) - 8,
		           (npc[gEditorNPC].tgt_y / 0x400) / 16);
	}
}

void EditorProc(NPCHAR *npc)
{
	switch (gEditorMode)
	{
		case FALSE:
			if (gMouseTrg2 & MOUSE_LEFT)
			{
				if (gEditorCursor_ScreenPos.x <= 8 ||
					gEditorCursor_ScreenPos.x >= 224 ||
					gEditorCursor_ScreenPos.y <= 8 ||
					gEditorCursor_ScreenPos.y >= 40)
				{
					//Not in frame, move NPC and start dragging target
					PlaySoundObject(SOUND_ID_READY, SOUND_MODE_PLAY);
					gEditorMode = 1;
					gEditorCursor_NPC.x = gEditorCursor_Tile.x;
					gEditorCursor_NPC.y = gEditorCursor_Tile.y;
					npc[gEditorNPC].cond = TRUE;
					npc[gEditorNPC].x = gEditorCursor_NPC.x << 14;
					npc[gEditorNPC].y = gEditorCursor_NPC.y << 14;
				}
				else
				{
					//In frame
					//No. clicked
					if (gEditorCursor_ScreenPos.x > 60 &&
					    gEditorCursor_ScreenPos.x < 84 &&
					    gEditorCursor_ScreenPos.y > 16 &&
					    gEditorCursor_ScreenPos.y < 24)
					{
						//Increment selected NPC
						PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
						if (++gEditorNPC >= MAX_NPCS)
							gEditorNPC = 0;
						gEditorCursor_NPC.x = (npc[gEditorNPC].x / 16) / 0x400;
						gEditorCursor_NPC.y = (npc[gEditorNPC].y / 16) / 0x400;
					}
					//Character clicked
					if (gEditorCursor_ScreenPos.x > 60 &&
					    gEditorCursor_ScreenPos.x < 84 &&
					    gEditorCursor_ScreenPos.y > 28 &&
					    gEditorCursor_ScreenPos.y < 36)
					{
						//Increment selected NPC's character
						PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
						if (++npc[gEditorNPC].code_char >= 10)
							npc[gEditorNPC].code_char = 0;
					}
					//Event clicked
					if (gEditorCursor_ScreenPos.x > 148 &&
					    gEditorCursor_ScreenPos.x < 172 &&
					    gEditorCursor_ScreenPos.y > 28 &&
					    gEditorCursor_ScreenPos.y < 36)
					{
						//Increment selected NPC's event
						PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
						if (++npc[gEditorNPC].code_event >= 256)
							npc[gEditorNPC].code_event = 0;
					}
					//Type clicked
					if (gEditorCursor_ScreenPos.x > 192 &&
					    gEditorCursor_ScreenPos.x < 216 &&
					    gEditorCursor_ScreenPos.y > 16 &&
					    gEditorCursor_ScreenPos.y < 24)
					{
						//Increment selected NPC's type
						PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
						if (++npc[gEditorNPC].type >= 4)
							npc[gEditorNPC].type = 0;
					}
				}
			}
			else if (gMouseTrg2 & MOUSE_RIGHT)
			{
				if (gEditorCursor_ScreenPos.x <= 8 ||
					gEditorCursor_ScreenPos.x >= 224 ||
					gEditorCursor_ScreenPos.y <= 8 ||
					gEditorCursor_ScreenPos.y >= 40)
				{
					//Not in frame, delete NPC mouse is over
					for (int i = 0; i < MAX_NPCS; i++)
					{
						if (gEditorCursor_Tile.x == ((npc[i].x / 0x400) / 16) && gEditorCursor_Tile.y == ((npc[i].y / 0x400) / 16))
						{
							//Delete NPC
							PlaySoundObject(SOUND_ID_OUCH, SOUND_MODE_PLAY);
							npc[i].cond = FALSE;
						}
					}
				}
				else
				{
					//In frame
					//No. clicked
					if (gEditorCursor_ScreenPos.x > 60 &&
					    gEditorCursor_ScreenPos.x < 84 &&
					    gEditorCursor_ScreenPos.y > 16 &&
					    gEditorCursor_ScreenPos.y < 24)
					{
						//Decrement selected NPC
						PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
						if (--gEditorNPC < 0)
							gEditorNPC = (MAX_NPCS - 1);
						gEditorCursor_NPC.x = (npc[gEditorNPC].x / 16) / 0x400;
						gEditorCursor_NPC.y = (npc[gEditorNPC].y / 16) / 0x400;
					}
					//Character clicked
					if (gEditorCursor_ScreenPos.x > 60 &&
					    gEditorCursor_ScreenPos.x < 84 &&
					    gEditorCursor_ScreenPos.y > 28 &&
					    gEditorCursor_ScreenPos.y < 36)
					{
						//Increment selected NPC's character
						PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
						if (--npc[gEditorNPC].code_char < 0)
							npc[gEditorNPC].code_char = 9;
					}
					//Event clicked
					if (gEditorCursor_ScreenPos.x > 148 &&
					    gEditorCursor_ScreenPos.x < 172 &&
					    gEditorCursor_ScreenPos.y > 28 &&
					    gEditorCursor_ScreenPos.y < 36)
					{
						//Decrement selected NPC's event
						PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
						if (--npc[gEditorNPC].code_event < 0)
							npc[gEditorNPC].code_event = 255;
					}
					//Type clicked
					if (gEditorCursor_ScreenPos.x > 192 &&
					    gEditorCursor_ScreenPos.x < 216 &&
					    gEditorCursor_ScreenPos.y > 16 &&
					    gEditorCursor_ScreenPos.y < 24)
					{
						//Decrement selected NPC's type
						PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
						if (--npc[gEditorNPC].type < 0)
							npc[gEditorNPC].type = 3;
					}
				}
			}
			break;
		case TRUE:
			//Drag target
			npc[gEditorNPC].tgt_x = gEditorCursor_Tile.x << 14;
			npc[gEditorNPC].tgt_y = gEditorCursor_Tile.y << 14;
			if ((gMouse & MOUSE_LEFT) == 0)
			{
				PlaySoundObject(SOUND_ID_GO, SOUND_MODE_PLAY);
				gEditorMode = 0;
			}
			break;
	}
}

void PutEditorNpcInfo(NPCHAR *npc)
{
	static RECT rcFrame = { 0, 0, 216, 32 };
	static RECT rcNpcView = { 0, 0, 32, 32 };
	static RECT rcNpcType[4] = {
		{ 0,  0, 24,  8 },
		{ 0,  8, 24, 16 },
		{ 0, 16, 24, 24 },
		{ 0, 24, 24, 32 },
	};
	
	//Draw frame
	PutBitmap3(&grcFull, 8, 8, &rcFrame, 15);
	PutBitmap3(&grcFull, 8, 8, &rcNpcView, SURFACE_ID_HARI + npc[gEditorNPC].code_char);
	
	//Draw NPC values
	PutNumber2(60, 16, gEditorNPC);
	PutNumber2(60, 28, npc[gEditorNPC].code_char);
	PutNumber2(104, 16, (npc[gEditorNPC].x / 16) / 0x400);
	PutNumber2(104, 28, (npc[gEditorNPC].y / 16) / 0x400);
	PutNumber2(148, 16, npc[gEditorNPC].cond);
	PutNumber2(148, 28, npc[gEditorNPC].code_event);
	
	//Draw NPC type
	PutBitmap3(&grcFull, 192, 16, &rcNpcType[npc[gEditorNPC].type], SURFACE_ID_NPCTYPE);
}

BOOL LoadNpChar(NPCHAR *npc)
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
		fread(&npc[i].cond, 1, 1, fp);
		fread(&npc[i].type, 1, 1, fp);
		fread(&npc[i].code_char, 1, 1, fp);
		fread(&npc[i].code_event, 2, 1, fp);
		fread(&npc[i].direct, 1, 1, fp);
		fread(&npc[i].x, 4, 1, fp);
		fread(&npc[i].y, 4, 1, fp);
		fread(&npc[i].tgt_x, 4, 1, fp);
		fread(&npc[i].tgt_y, 4, 1, fp);
	}
	return TRUE;
}

BOOL SaveNpChar(NPCHAR *npc)
{
	//Open NPChar.dat
	TCHAR path[MAX_PATH];
	sprintf(path, "%s\\%s", gModulePath, "NPChar.dat");
	
	FILE *fp = fopen(path, "wb");
	if (fp == NULL)
		return FALSE;
	
	//Read NPC data
	for (int i = 0; i < MAX_NPCS; i++)
	{
		fwrite(&npc[i].cond, 1, 1, fp);
		fwrite(&npc[i].type, 1, 1, fp);
		fwrite(&npc[i].code_char, 1, 1, fp);
		fwrite(&npc[i].code_event, 2, 1, fp);
		fwrite(&npc[i].direct, 1, 1, fp);
		fwrite(&npc[i].x, 4, 1, fp);
		fwrite(&npc[i].y, 4, 1, fp);
		fwrite(&npc[i].tgt_x, 4, 1, fp);
		fwrite(&npc[i].tgt_y, 4, 1, fp);
	}
	return TRUE;
}
