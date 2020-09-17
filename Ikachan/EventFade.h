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

BOOL ProcFade(FADE1 *fade);
