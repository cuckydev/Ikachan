#pragma once
#include <windows.h>

#define FLAG_BYTES (32)
extern BYTE gFlagNPC[FLAG_BYTES];

void InitFlags();
BOOL SetNPCFlag(LONG a);
BOOL CutNPCFlag(LONG a);
BOOL GetNPCFlag(LONG a);
