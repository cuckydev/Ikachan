#pragma once
#include <windows.h>

enum WND_SIZE
{
	WS_FULLSCREEN,
	WS_320x240,
	WS_640x480,
};

extern WND_SIZE gWndSize;
extern TCHAR gModulePath[MAX_PATH];

int Random(int min, int max);
BOOL SystemTask();
