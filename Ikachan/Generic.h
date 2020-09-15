#pragma once
#include <windows.h>

void GetCompileDate(int *year, int *month, int *day);
BOOL GetCompileVersion(int *v1, int *v2, int *v3, int *v4);
BOOL OpenSoundVolume(HWND hWnd);
void LoadGenericData();
