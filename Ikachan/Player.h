#pragma once
#include <windows.h>

struct MYCHAR
{
	BOOLEAN cond;
	char unit;
	char equip;
	int x;
	int y;
	int xm;
	int ym;
	char ani_no;
	short ani_wait;
	char direct;
	short act_wait;
	short dash_wait;
};

extern MYCHAR gMC;

void PutMyChar();
//void PutMyStatus();
void ActMyChar_Normal(BOOL bKey);
//void ActMyChar_Unit1(BOOL bKey);
//void ActMyChar_Unit2(BOOL bKey);
void ActMyChar(BOOL bKey);
void InitMyChar();