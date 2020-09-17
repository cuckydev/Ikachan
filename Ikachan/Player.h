#pragma once
#include <windows.h>

struct MYCHAR
{
	BOOLEAN cond; //x0 x420DD8
	char x1; //x1 x420DD9
	char unit; //x2 x420DDA
	char equip; //x3 x420DDB
	int x; //x4 x420DDC
	int y; //x8 x420DE0
	int xm; //xC x420DE4
	int ym; //x10 x420DE8
	char ani_no; //x14 x420DEC
	//alignment x15
	short ani_wait; //x16 x420DEE
	short x18; //x18 x420DF0
	short x1A; //x1A x420DF2
	short life; //x1C x420DF4
	short exp; //x1E x420DF6
	char level; //x20 x420DF8
	//alignment x21
	short swim_wait; //x22 x420DFA
	short dash_wait; //x24 x420DFC
	char direct; //x26 x420DFE
	char x27; //x27 x420DFF
	char x28; //x28 x420DE0
	char x29; //x29 x420DE1
};

extern MYCHAR gMC;

#define MAX_LEVEL (7)
extern short gMycLife[MAX_LEVEL];
extern short gMycExp[MAX_LEVEL];

void PutMyChar();
void PutMyStatus();
void ActMyChar();
void InitMyChar();