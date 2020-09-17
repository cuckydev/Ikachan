#pragma once
#include <windows.h>

struct MAP
{
	BYTE *data;
	int width, length;
};

BOOL LoadMapData(LPCTSTR path, MAP *map);
