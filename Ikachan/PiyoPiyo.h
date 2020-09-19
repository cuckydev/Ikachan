#pragma once
#include <windows.h>

struct PIYOPIYO_CONTROL
{
	char mode; //x0
	char track; //x1
	char prev_track; //x2
	//alignment x3
	short volume; //x4
};

BOOL InitPiyoPiyo();
void EndPiyoPiyo();
void PiyoPiyoProc();
void PiyoPiyoControl(PIYOPIYO_CONTROL *piyocont);
