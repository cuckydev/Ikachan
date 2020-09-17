#pragma once
#include <windows.h>

struct OPENING
{
	int x0;
	int x4;
	int x8;
	int xC;
};

void InitOpening(OPENING *op);
void PutOpening(OPENING *op);
