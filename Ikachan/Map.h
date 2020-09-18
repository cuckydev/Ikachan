#pragma once
#include "NpChar.h"
#include <windows.h>

struct MAP
{
	BYTE *data;
	BYTE fx;
	int width, length;
};

enum FRAME_MODE
{
	FRAME_MODE_MYCHAR,
	FRAME_MODE_NPCHAR,
	FRAME_MODE_BOSS,
};

struct FRAME
{
	char mode;
	short npc;
	int x, y;
};

BOOL LoadMapData(LPCTSTR path, MAP *map);
void PutBack(FRAME *frame);
void PutMapBack(MAP *map, int fx, int fy);
void PutMapFront(MAP *map, int fx, int fy);
void PutMapVector(MAP *map, int fx, int fy);
void MoveFrame(FRAME *frame, NPCHAR *npc, MAP *map);
void MoveFrameEditor(FRAME *frame, MAP *map);
