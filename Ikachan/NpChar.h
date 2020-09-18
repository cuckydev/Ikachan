#pragma once
#include <windows.h>

#define MAX_NPCS (100)

struct NPCHAR
{
	BYTE cond; //x0
	BYTE x1; //x1
	char code_char; //x2
	WORD x4; //x4
	WORD x6; //x6
	char act_no; //x8
	char ani_no; //x9
	short ani_wait; //xA
	char direct; //xC
	int x; //x10
	int y; //x14
	int xm; //x18
	int ym; //x1C
	int tgt_x; //x20
	int tgt_y; //x24
};

void InitNpChar(NPCHAR *npc);
