#include "EventScript.h"
#include "Draw.h"
#include "System.h"
#include "Sound.h"
#include "Flags.h"
#include "Player.h"
#include "Game.h"
#include "Editor.h"
#include <stdio.h>

#define IS_COMMAND(c1, c2) (ptx->data[ptx->p_read] == '<' && ptx->data[ptx->p_read + 1] == (c1) && ptx->data[ptx->p_read + 2] == (c2))

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

RECT rcNumber2[10] = {
	{  0, 8,  4, 16 },
	{  4, 8,  8, 16 },
	{  8, 8, 12, 16 },
	{ 12, 8, 16, 16 },
	{ 16, 8, 20, 16 },
	{ 20, 8, 24, 16 },
	{ 24, 8, 28, 16 },
	{ 28, 8, 32, 16 },
	{ 32, 8, 36, 16 },
	{ 36, 8, 40, 16 },
};

RECT rcLine = { 0, 0, 272, 16 };
RECT rcYNYes = { 0, 0, 48, 16 };
RECT rcYNNo = { 0, 16, 48, 32 };
RECT rcYNSel = { 48, 0, 112, 32 };
RECT rcLineClip = { (SURFACE_WIDTH / 2) - 136, SURFACE_HEIGHT - 50, (SURFACE_WIDTH / 2) + 136, SURFACE_HEIGHT - 14 };

//Fade dimensions
#define FADE_WIDTH ((SURFACE_WIDTH + 15) / 16)
#define FADE_HEIGHT ((SURFACE_HEIGHT + 15) / 16)

//Fading and other screen effects
BOOL ProcFade(FADE *fade, FRAME *frame)
{
	switch (fade->mode)
	{
		case FADE_MODE_NONE:
			fade->wait = 0;
			break;
		case FADE_MODE_QUAKE:
			if (++fade->wait <= 60)
			{
				//Shake screen
				frame->x += Random(-10, 10) << 10;
				frame->y += Random(-10, 10) << 10;
				
				//Create quake particles
			}
			break;
		case FADE_MODE_FADEOUT:
			for (int y = 0; y < FADE_HEIGHT; y++)
			{
				for (int x = 0; x < FADE_WIDTH; x++)
				{
					int frame = fade->wait - y - x;
					if (frame < 0)
						frame = 0;
					if (frame > 15)
						frame = 15;
					PutBitmap3(&grcFull, x * 16, ((FADE_HEIGHT - 1) - y) * 16, &rcFade[frame], SURFACE_ID_FADE);
				}
			}
			if (++fade->wait <= 50)
				break;
			fade->mode = 0;
			return TRUE;
		case FADE_MODE_FADEIN:
			for (int y = 0; y < FADE_HEIGHT; y++)
			{
				for (int x = 0; x < FADE_WIDTH; x++)
				{
					int frame = fade->wait - y - x;
					if (frame < 0)
						frame = 0;
					if (frame > 15)
						frame = 15;
					PutBitmap3(&grcFull, x * 16, ((FADE_HEIGHT - 1) - y) * 16, &rcFade[15 - frame], SURFACE_ID_FADE);
				}
			}
			if (++fade->wait <= 50)
				break;
			fade->mode = 0;
			return TRUE;
		case FADE_MODE_QUAKE2:
			if (!(++fade->wait % 4))
			{
				//Shake screen
				frame->x += Random(-10, 10) << 10;
				frame->y += Random(-10, 10) << 10;
				
				//Create quake particles
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
			PutBitmap3(&grcFull, x + (4 * i), y, &rcNumber2[v4], SURFACE_ID_FIGURE);
	}
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
	ptx->data = (LPSTR)LocalAlloc(LPTR, ptx->size + 1);
	
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

BOOL SaveRecord(ITEMS *items, MAP *map, NPCHAR *npc)
{
	//Open file
	TCHAR path[MAX_PATH];
	sprintf(path, "%s\\%s", gModulePath, "Ika.rec");
	
	FILE *fp = fopen(path, "wb");
	if (fp == NULL)
		return FALSE;
	
	//Write magic
	fwrite(save_magic, 1, 8, fp);
	
	//Write Ikachan's state
	fwrite(&gMC.x, 4, 1, fp);
	fwrite(&gMC.y, 4, 1, fp);
	fwrite(&gMC.life, 2, 1, fp);
	fwrite(&gMC.exp, 2, 1, fp);
	fwrite(&gMC.level, 1, 1, fp);
	fwrite(&gMC.equip, 1, 1, fp);
	
	//Write flags
	fwrite(gFlagNPC, 1, FLAG_BYTES, fp);
	
	//Write items
	fwrite(items->code, 1, MAX_ITEMS, fp);
	
	//Write map data
	fwrite(&map->width, 4, 1, fp);
	fwrite(&map->length, 4, 1, fp);
	fwrite(map->data, 1, map->length * map->width, fp);
	
	//Write NPCs
	for (int i = 0; i < MAX_NPCS; i++)
	{
		fwrite(&npc[i].x, 4, 1, fp);
		fwrite(&npc[i].y, 4, 1, fp);
		fwrite(&npc[i].tgt_x, 4, 1, fp);
		fwrite(&npc[i].tgt_y, 4, 1, fp);
		fwrite(&npc[i].xm, 4, 1, fp);
		fwrite(&npc[i].ym, 4, 1, fp);
	}
	
	fclose(fp);
	return TRUE;
}

BOOL LoadRecord(ITEMS *items, MAP *map, NPCHAR *npc)
{
	//Open file
	TCHAR path[MAX_PATH];
	sprintf(path, "%s\\%s", gModulePath, "Ika.rec");
	
	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
		return FALSE;
	
	//Verify magic
	char magic[8];
	fread(magic, 1, 8, fp);
	for (int i = 0; i < 8; i++)
		if (magic[i] != save_magic[i])
			return FALSE; //strncmp will be real in
	
	//Read Ikachan's state
	fread(&gMC.x, 4, 1, fp);
	fread(&gMC.y, 4, 1, fp);
	fread(&gMC.life, 2, 1, fp);
	fread(&gMC.exp, 2, 1, fp);
	fread(&gMC.level, 1, 1, fp);
	fread(&gMC.equip, 1, 1, fp);
	
	//Read flags
	fread(gFlagNPC, 1, FLAG_BYTES, fp);
	
	//Read items
	fread(items->code, 1, MAX_ITEMS, fp);
	
	//Read map data
	fread(&map->width, 4, 1, fp);
	fread(&map->length, 4, 1, fp);
	fread(map->data, 1, map->length * map->width, fp);
	
	//Read NPCs
	for (int i = 0; i < MAX_NPCS; i++)
	{
		fread(&npc[i].x, 4, 1, fp);
		fread(&npc[i].y, 4, 1, fp);
		fread(&npc[i].tgt_x, 4, 1, fp);
		fread(&npc[i].tgt_y, 4, 1, fp);
		fread(&npc[i].xm, 4, 1, fp);
		fread(&npc[i].ym, 4, 1, fp);
	}
	
	fclose(fp);
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
			if (no > ptx->event_no)
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
		{ 56, 16, 64, 32 },
		{ 48, 16, 56, 32 },
		{ 48, 16, 56, 32 },
		{ 56, 16, 64, 32 },
		{ 64, 16, 72, 32 },
		{ 72, 16, 80, 32 },
		{ 64,  0, 72, 16 },
		{ 72,  0, 80, 16 },
	};
	PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) + 132, SURFACE_HEIGHT - 26, &rcCursor[(ptx->ani_cursor >> 2) % 8], SURFACE_ID_CURSOR);
}

char EventScriptProc(EVENT_SCR *ptx, ITEMS *items, NPCHAR *npc, MAP *map, PIYOPIYO_CONTROL *piyocont, FADE *fade, FRAME *frame)
{
	TCHAR c[3] = { 0 };

	switch (ptx->mode)
	{
		case 1:
			if (JumpEventScript(ptx))
			{
				//Initialize script
				ptx->wait = 0;
				ptx->line = 0;
				ptx->ypos_line[0] = 0;
				ptx->ypos_line[1] = 20;
				ptx->p_write = 0;
				CortBox2(&rcLine, 0x000000, SURFACE_ID_TEXT0);
				CortBox2(&rcLine, 0x000000, SURFACE_ID_TEXT1);
				ptx->mode = 2;
			}
			else
			{
				//Don't run script
				ptx->mode = 0;
			}
			return 0;
		case 2:
			//Wait until timer is done before resuming execution
			if (--ptx->wait >= 0)
				return 0;
			break;
		case 4:
			//Scroll lines
			ptx->ypos_line[0] -= 4;
			ptx->ypos_line[1] -= 4;
			
			//Check if we should stop
			if (ptx->ypos_line[1] != 0)
			{
				if (ptx->ypos_line[0] == 0)
				{
					//Stopped on line 0
					CortBox2(&rcLine, 0x000000, SURFACE_ID_TEXT1);
					ptx->ypos_line[1] = 20;
					ptx->line++;
					ptx->mode = 2;
				}
			}
			else
			{
				//Stopped on line 1
				CortBox2(&rcLine, 0x000000, SURFACE_ID_TEXT0);
				ptx->ypos_line[0] = 20;
				ptx->line++;
				ptx->mode = 2;
			}
			return 0;
		case 5:
			//Wait until Z is pressed before resuming execution
			if (gKeyTrg & KEY_Z)
				ptx->mode = 4;
			return 0;
		case 6:
			//Wait until Z is pressed before resuming execution
			if (gKeyTrg & KEY_Z)
			{
				ptx->wait = 0;
				ptx->line = 0;
				ptx->ypos_line[0] = 0;
				ptx->ypos_line[1] = 20;
				ptx->p_write = 0;
				CortBox2(&rcLine, 0x000000, SURFACE_ID_TEXT0);
				CortBox2(&rcLine, 0x000000, SURFACE_ID_TEXT1);
				ptx->mode = 2;
			}
			return 0;
		case 7:
			//Wait until Z is pressed before ending the script
			if (!(gKeyTrg & KEY_Z))
				return 0;
			ptx->mode = 0;
			ptx->msg_box = 0;
			return 0;
		case 8:
			//End script
			ptx->mode = 0;
			ptx->msg_box = 0;
			return 0;
		case 9:
			//Draw Yes/No dialogue
			PutBitmap3(&grcFull, ((SURFACE_WIDTH / 2) - 81) + (ptx->select * 96), (SURFACE_HEIGHT / 2) - 27, &rcYNSel, SURFACE_ID_YESNO);
			PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 72, (SURFACE_HEIGHT / 2) - 20, &rcYNYes, SURFACE_ID_YESNO);
			PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) + 24, (SURFACE_HEIGHT / 2) - 20, &rcYNNo, SURFACE_ID_YESNO);
			
			//Choose Yes/No
			if (gKeyTrg & KEY_LEFT)
			{
				ptx->select = 0;
				PlaySoundObject(SOUND_ID_READY, 1);
			}
			if (gKeyTrg & KEY_RIGHT)
			{
				ptx->select = 1;
				PlaySoundObject(SOUND_ID_READY, 1);
			}
			
			//Select once Z is pressed
			if (gKeyTrg & KEY_Z)
			{
				PlaySoundObject(SOUND_ID_DASH, 1);
				if (ptx->select)
				{
					//No
					ptx->p_read += 4;
					ptx->wait = 0;
					ptx->line = 0;
					ptx->ypos_line[0] = 0;
					ptx->ypos_line[1] = 20;
					ptx->p_write = 0;
					CortBox2(&rcLine, 0x000000, SURFACE_ID_TEXT0);
					CortBox2(&rcLine, 0x000000, SURFACE_ID_TEXT1);
					ptx->mode = 2;
				}
				else
				{
					//Yes
					ptx->event_no = GetEventScriptNo(ptx);
					ptx->mode = 1;
				}
			}
			return 0;
		default:
			return 0;
	};
	
	//Parse script
	short x, y;

	while (ptx->p_read < ptx->size)
	{
		//Skip past this specific character
		if (ptx->data[ptx->p_read] == '~')
			ptx->p_read++;
		
		//Print Shift-JIS
		if (ptx->data[ptx->p_read] & 0x80)
		{
			//Type wait
			ptx->msg_box = 1;
			ptx->wait = ptx->x1C;
			if (gKey & KEY_Z)
				ptx->wait = 0;
			
			//Type character
			c[0] = ptx->data[ptx->p_read];
			c[1] = ptx->data[ptx->p_read + 1];
			PlaySoundObject(SOUND_ID_MESSAGE, SOUND_MODE_PLAY);
			PutText2((8 * ptx->p_write) + 1, 1, c, 0xFF0000, SURFACE_ID_TEXT0 + (ptx->line % 2));
			PutText2(8 * ptx->p_write, 0, c, 0xFFFFFF, SURFACE_ID_TEXT0 + (ptx->line % 2));
			ptx->p_write += 2;
			ptx->p_read += 2;
			return 0;
		}
		
		//Print English alphabet
		if (ptx->data[ptx->p_read] >= 'A' && ptx->data[ptx->p_read] <= 'z')
		{
			//Type wait
			ptx->msg_box = 1;
			ptx->wait = ptx->x1C;
			if (gKey & KEY_Z)
				ptx->wait = 0;
			
			//Type character
			c[0] = ptx->data[ptx->p_read];
			c[1] = 0;
			PlaySoundObject(SOUND_ID_MESSAGE, SOUND_MODE_PLAY);
			PutText2((8 * ptx->p_write) + 1, 1, c, 0xFF0000, SURFACE_ID_TEXT0 + (ptx->line % 2));
			PutText2(8 * ptx->p_write, 0, c, 0xFFFFFF, SURFACE_ID_TEXT0 + (ptx->line % 2));
			ptx->p_write++;
			ptx->p_read++;
			return 0;
		}
		
		//Print English numerals
		if (ptx->data[ptx->p_read] >= '0' && ptx->data[ptx->p_read] <= '9')
		{
			//Type wait
			ptx->msg_box = 1;
			ptx->wait = ptx->x1C;
			if (gKey & KEY_Z)
				ptx->wait = 0;
			
			//Type character
			c[0] = ptx->data[ptx->p_read];
			c[1] = 0;
			PlaySoundObject(SOUND_ID_MESSAGE, SOUND_MODE_PLAY);
			PutText2((8 * ptx->p_write) + 1, 1, c, 0xFF0000, SURFACE_ID_TEXT0 + (ptx->line % 2));
			PutText2(8 * ptx->p_write, 0, c, 0xFFFFFF, SURFACE_ID_TEXT0 + (ptx->line % 2));
			ptx->p_write++;
			ptx->p_read++;
			return 0;
		}
		
		//Handle specific characters
		switch (ptx->data[ptx->p_read])
		{
			case ',':
			case '.':
			case '/':
				//Type wait
				ptx->msg_box = 1;
				ptx->wait = ptx->x1C;
				if (gKey & KEY_Z)
					ptx->wait = 0;
				
				//Type character
				c[0] = ptx->data[ptx->p_read];
				c[1] = 0;
				PlaySoundObject(SOUND_ID_MESSAGE, SOUND_MODE_PLAY);
				PutText2((8 * ptx->p_write) + 1, 1, c, 0xFF0000, SURFACE_ID_TEXT0 + (ptx->line % 2));
				PutText2(8 * ptx->p_write, 0, c, 0xFFFFFF, SURFACE_ID_TEXT0 + (ptx->line % 2));
				ptx->p_write++;
				ptx->p_read++;
				return 0;
			case '+':
				//Skip character
				ptx->wait = ptx->x1C;
				ptx->p_read += 2;
				ptx->wait = 0; //lol
				
				//Start scrolling / go to next line
				ptx->p_write = 0;
				if ((ptx->line % 2) || ptx->ypos_line[1] != 0)
				{
					if ((ptx->line % 2) != 1 || ptx->ypos_line[0] != 0)
						++ptx->line;
					else
						ptx->mode = 5;
				}
				else
				{
					ptx->mode = 5;
				}
				return 0;
			case '{':
				//'NOD'
				ptx->p_read += 2;
				ptx->mode = 6;
				return 0;
		}

		//Handle commands
		if (IS_COMMAND('f','+'))
		{
			//Set flag
			ptx->p_read += 3;
			x = GetEventScriptNo(ptx);
			SetNPCFlag(x);
			return 0;
		}
		if (IS_COMMAND('f','-'))
		{
			//Clear flag
			ptx->p_read += 3;
			x = GetEventScriptNo(ptx);
			CutNPCFlag(x);
			return 0;
		}
		if (IS_COMMAND('f','j'))
		{
			//Check if flag is set
			ptx->p_read += 3;
			x = GetEventScriptNo(ptx);
			if (GetNPCFlag(x) == TRUE)
			{
				//Jump to event given
				ptx->p_read++;
				ptx->event_no = GetEventScriptNo(ptx);
				ptx->mode = 1;
			}
			else
			{
				//Continue without jumping
				ptx->p_read += 5;
			}
			return 0;
		}
		if (IS_COMMAND('i','+'))
		{
			//Give item
			PlaySoundObject(SOUND_ID_ITEM, SOUND_MODE_PLAY);
			ptx->p_read += 3;
			x = GetEventScriptNo(ptx);
			AddItemData(items, (char)x);
			return 0;
		}
		if (IS_COMMAND('i','-'))
		{
			//Remove item
			ptx->p_read += 3;
			x = GetEventScriptNo(ptx);
			SubItemData(items, (char)x);
			return 0;
		}
		if (IS_COMMAND('i','j'))
		{
			//Check if we have requested item
			ptx->p_read += 3;
			x = GetEventScriptNo(ptx);
			if (CheckItem(items, (char)x) == TRUE)
			{
				//Jump to event given
				ptx->p_read++;
				ptx->event_no = GetEventScriptNo(ptx);
				ptx->mode = 1;
			}
			else
			{
				//Continue without jumping
				ptx->p_read += 5;
			}
			return 0;
		}
		if (IS_COMMAND('p','c'))
		{
			//Modify map
			ptx->p_read += 3;
			x = GetEventScriptNo(ptx);
			ptx->p_read++;
			y = GetEventScriptNo(ptx);
			ptx->p_read++;
			map->data[x + map->width * y] = (BYTE)GetEventScriptNo(ptx);
			return 0;
		}
		if (IS_COMMAND('c','m'))
		{
			//Get arguments
			ptx->p_read += 3;
			int z = GetEventScriptNo(ptx);
			ptx->p_read++;
			x = GetEventScriptNo(ptx);
			ptx->p_read++;
			y = GetEventScriptNo(ptx);
			ptx->p_read++;

			//Set NPC
			short xm = x - (npc[z].x / 16 / 0x400);
			short ym = y - (npc[z].y / 16 / 0x400);
			npc[z].x = x << 14;
			npc[z].y = y << 14;
			npc[z].tgt_x += xm << 14;
			npc[z].tgt_y += ym << 14;
		}
		if (IS_COMMAND('s','e'))
		{
			//Play requested sound
			ptx->p_read += 3;
			x = GetEventScriptNo(ptx);
			PlaySoundObject(x, SOUND_MODE_PLAY);
			return 0;
		}
		if (IS_COMMAND('w','a'))
		{
			//Wait requested time
			ptx->p_read += 3;
			ptx->wait = GetEventScriptNo(ptx);
			return 0;
		}
		if (IS_COMMAND('e','k'))
		{
			//Quake
			ptx->p_read += 3;
			fade->wait = 0;
			fade->mode = FADE_MODE_QUAKE;
			return 0;
		}
		if (IS_COMMAND('l','e'))
		{
			//Quake 2
			ptx->p_read += 3;
			fade->wait = 0;
			fade->mode = FADE_MODE_QUAKE2;
			return 0;
		}
		if (IS_COMMAND('f','o'))
		{
			//Fade out
			ptx->p_read += 3;
			fade->wait = 0;
			fade->mode = FADE_MODE_FADEOUT;
			return 0;
		}
		if (IS_COMMAND('f','i'))
		{
			//Fade in
			ptx->p_read += 3;
			fade->wait = 0;
			fade->mode = FADE_MODE_FADEIN;
			return 0;
		}
		if (IS_COMMAND('m','a'))
		{
			//Fade mask
			ptx->p_read += 3;
			fade->mask = TRUE;
			return 0;
		}
		if (IS_COMMAND('n','m'))
		{
			//Fade unmask
			ptx->p_read += 3;
			fade->mask = FALSE;
			return 0;
		}
		if (IS_COMMAND('y','n'))
		{
			//Start Yes/No dialogue
			ptx->p_read += 3;
			ptx->select = 0;
			ptx->mode = 9;
			PlaySoundObject(SOUND_ID_YESNO, SOUND_MODE_PLAY);
			return 0;
		}
		if (IS_COMMAND('m','c'))
		{
			//Close message box
			ptx->p_read += 3;
			ptx->msg_box = FALSE;
			return 0;
		}
		if (IS_COMMAND('d','s'))
		{
			//Save game
			PlaySoundObject(SOUND_ID_SAVE, SOUND_MODE_PLAY);
			SaveRecord(items, map, npc);
			ptx->p_read += 3;
			return 0;
		}
		if (IS_COMMAND('d','l'))
		{
			//Load game
			PlaySoundObject(SOUND_ID_SAVE, SOUND_MODE_PLAY);
			if (LoadRecord(items, map, npc))
			{
				//Start from loaded game
				gMC.no_event = 100;
				ptx->p_read += 3;
				return 2;
			}
			else
			{
				//Failed to load
				ptx->p_read += 3;
				return 1;
			}
		}
		if (IS_COMMAND('n','l'))
		{
			LoadNpChar(npc);
			gMC.no_event = 100;
			ptx->p_read += 3;
			return 0;
		}
		if (IS_COMMAND('c','e'))
		{
			ReloadNpChar(npc);
			//InitBoss();
			ptx->p_read += 3;
			return 0;
		}
		if (IS_COMMAND('e','+'))
		{
			//Restore Ikachan's health
			gMC.life = gMycLife[gMC.level];
			ptx->p_read += 3;
			PlaySoundObject(SOUND_ID_LIFEUP, SOUND_MODE_PLAY);
			return 0;
		}
		if (IS_COMMAND('g','e'))
		{
			//Escape in ship
			gMC.unit = 2;
			gMC.direct = 2;
			fade->mode = 0;
			ptx->p_read += 3;
			PlaySoundObject(SOUND_ID_LIFEUP, SOUND_MODE_PLAY);
			return 0;
		}
		if (IS_COMMAND('r','e'))
		{
			//Restart
			gMC.carry = 0;
			gMC.cond = 1;
			gMC.unit = 0;
			gMC.ani_no = 0;
			gMC.ani_wait = 0;
			gMC.shock = 0;
			gMC.no_event = 100;
			gMC.ym = 0;
			gMC.xm = 0;
			gMC.life = gMycLife[gMC.level];
			ptx->p_read += 3;
			return 0;
		}
		if (IS_COMMAND('e','c'))
		{
			//Add 1 to 'people carried count'
			gMC.carry++;
			ptx->p_read += 3;
			return 0;
		}
		if (IS_COMMAND('e','j'))
		{
			//Check if we're carrying the right amount of people
			ptx->p_read += 3;
			if (gMC.carry == GetEventScriptNo(ptx))
			{
				//Jump to event given
				ptx->p_read++;
				ptx->event_no = GetEventScriptNo(ptx);
				ptx->mode = 1;
			}
			else
			{
				//Continue without jumping
				ptx->p_read += 5;
			}
			return 0;
		}
		if (IS_COMMAND('f','t'))
		{
			//Heal Ikachan and change frame focus
			gMC.life = gMycLife[gMC.level];
			ptx->p_read += 3;
			frame->mode = (char)GetEventScriptNo(ptx);
			ptx->p_read++;
			frame->npc = GetEventScriptNo(ptx);
			return 0;
		}
		if (IS_COMMAND('b','o'))
		{
			ptx->p_read += 3;
			GetEventScriptNo(ptx);
			//byte_41D88E = 1;
			return 0;
		}
		if (IS_COMMAND('p','p'))
		{
			ptx->p_read += 3;
			piyocont->mode = 2;
			return 0;
		}
		if (IS_COMMAND('p','s'))
		{
			ptx->p_read += 3;
			piyocont->mode = 3;
			return 0;
		}
		if (IS_COMMAND('p','f'))
		{
			ptx->p_read += 3;
			piyocont->mode = 4;
			return 0;
		}
		if (IS_COMMAND('p','d'))
		{
			ptx->p_read += 3;
			piyocont->track = (char)GetEventScriptNo(ptx);
			piyocont->mode = 1;
			PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 16, (SURFACE_HEIGHT / 2) - 4, &grcLoading, SURFACE_ID_LOADING);
			return 0;
		}
		if (IS_COMMAND('p','h'))
		{
			ptx->p_read += 3;
			piyocont->mode = 5;
			return 0;
		}
		if (IS_COMMAND('p','n'))
		{
			ptx->p_read += 3;
			piyocont->mode = 6;
			return 0;
		}
		if (IS_COMMAND('t','e'))
		{
			//'NOD', ends script
			ptx->mode = 7;
			return 0;
		}
		if (IS_COMMAND('e','n'))
		{
			//End script
			ptx->mode = 8;
			return 0;
		}
		if (IS_COMMAND('e','x'))
		{
			//Exit
			return 1;
		}
		if (ptx->data[ptx->p_read] == '<' && ptx->data[ptx->p_read + 1] == '*')
		{
			//Start credits
			return 3;
		}
		++ptx->p_read;
	}
	
	//End script
	ptx->mode = 8;
	return 0;
}

void PutMsgBox(EVENT_SCR *ptx)
{
	static RECT rcMsgBox = { 0, 0, 300, 48 };

	if (ptx->msg_box == TRUE)
	{
		++ptx->ani_cursor;
		PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 152, SURFACE_HEIGHT - 56, &rcMsgBox, SURFACE_ID_MSGBOX);
		PutBitmap3(&rcLineClip, (SURFACE_WIDTH / 2) - 136, (SURFACE_HEIGHT - 50) + ptx->ypos_line[0], &rcLine, SURFACE_ID_TEXT0);
		PutBitmap3(&rcLineClip, (SURFACE_WIDTH / 2) - 136, (SURFACE_HEIGHT - 50) + ptx->ypos_line[1], &rcLine, SURFACE_ID_TEXT1);
		if (ptx->mode == 5 || ptx->mode == 6 || ptx->mode == 7)
			PutEventScriptCursor(ptx);
	}
}

void InitEventScript(EVENT_SCR *ptx)
{
	ptx->mode = 0;
	ptx->msg_box = 0;
	ptx->event_no = 0;
	ptx->ani_cursor = 0;
	ptx->wait = 0;
	ptx->x1C = 4;
	ptx->line = 0;
	ptx->ypos_line[0] = 0;
	ptx->ypos_line[1] = 20;
	ptx->p_write = 0;
}
