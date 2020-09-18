#include "NpChar.h"

void InitNpChar(NPCHAR *npc)
{
	for (int i = 0; i < MAX_NPCS; i++)
	{
		npc[i].cond = 0;
		npc[i].x1 = 0;
		npc[i].code_char = 0;
		npc[i].x4 = 0;
		npc[i].x6 = 0;
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
