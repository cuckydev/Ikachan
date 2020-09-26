#pragma once
#include "Map.h"
#include "EventScript.h"
#include <windows.h>

struct BOSS
{
	BOOLEAN cond;
	short life;
	short shock;
	BYTE x6;
	char code_char;
	short code_event;
	short defeat_event;
	short act_wait;
	char act_no;
	char ani_no;
	short ani_wait;
	char direct;
	BYTE flag;
	BOOLEAN airborne;
	int x;
	int y;
	int xm;
	int ym;
	int tgt_x;
	int tgt_y;
	RECT hit;
};

extern BOSS gBoss;

void InitBoss();
void PutBoss(FRAME *frame);
void ActBoss(CARET_SPAWNER *caret_spawner);
void HitMyCharBoss(EVENT_SCR *event_scr, CARET_SPAWNER *caret_spawner);