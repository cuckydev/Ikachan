#include "Editor.h"
#include "System.h"
#include "Draw.h"
#include <stdio.h>

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
