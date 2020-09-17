#pragma once
#include <windows.h>

struct EVENT_SCR
{
	CHAR mode; //x0
	CHAR msg_box; //x1
	//x2
	//x3
	LPSTR data; //x4
	BYTE x8; //x8
	//x9
	//xA
	//xB
	LONG size; //xC
	int event_no; //x10
	int p_read; //x14
	BYTE ani_cursor; //x18
	//x19
	WORD x1A; //x1A
	WORD x1C; //x1C
	char line; //x1E
	char ypos_line[2]; //x1F, x20
	char x21; //x21
};

void DebugPutText(LPCTSTR text);
BOOL ReadEventScript(LPCTSTR path, EVENT_SCR *ptx);
BOOL JumpEventScript(EVENT_SCR *ptx);
short GetEventScriptNo(EVENT_SCR *ptx);
void PutEventScriptCursor(EVENT_SCR *ptx);
void PutMsgBox(EVENT_SCR *ptx);
void InitEventScript(EVENT_SCR *ptx);
