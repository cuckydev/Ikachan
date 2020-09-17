#pragma once
#include <windows.h>

enum FADEMODE
{
	FM_NONE,
	FM_QUAKE,
	FM_FADEOUT,
	FM_FADEIN,
	FM_QUAKE2,
};

struct FADE1
{
	BOOLEAN mask;
	BYTE mode;
	int time;
};

struct EVENT_SCR
{
	char mode; //x0
	char msg_box; //x1
	//alignment x2
	//alignment x3
	LPSTR data; //x4
	BYTE x8; //x8
	//alignment x9
	//alignment xA
	//alignment xB
	LONG size; //xC
	int event_no; //x10
	int p_read; //x14
	BYTE ani_cursor; //x18
	//alignment x19
	WORD x1A; //x1A
	WORD x1C; //x1C
	char line; //x1E
	char ypos_line[2]; //x1F, x20
	char x21; //x21
};

BOOL ProcFade(FADE1 *fade);
void PutNumber(int x, int y, int no);
void PutNumber2(int x, int y, int no);
void DebugPutText(LPCTSTR text);
BOOL ReadEventScript(LPCTSTR path, EVENT_SCR *ptx);
BOOL JumpEventScript(EVENT_SCR *ptx);
short GetEventScriptNo(EVENT_SCR *ptx);
void PutEventScriptCursor(EVENT_SCR *ptx);
void PutMsgBox(EVENT_SCR *ptx);
void InitEventScript(EVENT_SCR *ptx);
