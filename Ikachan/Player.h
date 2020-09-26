#pragma once
#include "Map.h"
#include "Effect.h"
#include <windows.h>

struct MYCHAR
{
	BOOLEAN cond; //x0 x420DD8
	BOOLEAN dead; //x1 x420DD9
	BYTE unit; //x2 x420DDA
	BYTE equip; //x3 x420DDB
	int x; //x4 x420DDC
	int y; //x8 x420DE0
	int xm; //xC x420DE4
	int ym; //x10 x420DE8
	char ani_no; //x14 x420DEC
	//alignment x15
	short ani_wait; //x16 x420DEE
	short shock; //x18 x420DF0
	short no_event; //x1A x420DF2
	short life; //x1C x420DF4
	short exp; //x1E x420DF6
	char level; //x20 x420DF8
	//alignment x21
	short swim_wait; //x22 x420DFA
	short dash_wait; //x24 x420DFC
	BYTE direct; //x26 x420DFE
	BOOLEAN airborne; //x27 x420DFF
	BYTE flag; //x28 x420EE0
	char carry; //x29 x420EE1
};

extern MYCHAR gMC;

#define MAX_LEVEL (6)
extern short gMycLife[MAX_LEVEL + 1];
extern short gMycExp[MAX_LEVEL + 1];

void DamageMyChar(CARET_SPAWNER *caret_spawner, char damage);
void HitMyCharMap(MAP *map, CARET_SPAWNER *caret_spawner);
void PutMyChar(FRAME *frame);
void PutMyStatus();
void ActMyChar(CARET *caret, CARET_SPAWNER *caret_spawner);
void InitMyChar();
