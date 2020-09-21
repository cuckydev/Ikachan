#pragma once
#include "Effect.h"
#include <windows.h>

#define MAX_NPCS (100)

struct NPCHAR
{
	BYTE cond; //x0
	BYTE type; //x1
	char code_char; //x2
	short code_event; //x4
	short act_wait; //x6
	char act_no; //x8
	char ani_no; //x9
	short ani_wait; //xA
	char direct; //xC
	BYTE flag; //xD
	BOOLEAN airborne; //xE
	int x; //x10
	int y; //x14
	int xm; //x18
	int ym; //x1C
	int tgt_x; //x20
	int tgt_y; //x24
};

struct MAP; //prototype
struct FRAME; //prototype
struct EVENT_SCR; //prototype

void InitNpChar(NPCHAR *npc);
void PutNpChar(NPCHAR *npc, FRAME *frame);
void ActNpChar(NPCHAR *npc);
void HitNpCharMap(NPCHAR *npc, MAP *map);
void HitMyCharNpChar(NPCHAR *npc, EVENT_SCR *event_scr, CARET_SPAWNER *caret_spawner);
BOOL ReloadNpChar(NPCHAR *npc);
