#include "Flags.h"

BYTE gFlagNPC[FLAG_BYTES];

void InitFlags()
{
	for (int i = 0; i < FLAG_BYTES; i++)
		gFlagNPC[i] = 0;
}

BOOL SetNPCFlag(LONG a)
{
	if (a > (FLAG_BYTES * 8))
		return FALSE;
	gFlagNPC[a / 8] |= 1 << a % 8;
	return TRUE;
}

BOOL CutNPCFlag(LONG a)
{
	if (a > (FLAG_BYTES * 8))
		return TRUE;
	gFlagNPC[a / 8] &= ~(1 << a % 8);
	return FALSE;
}

BOOL GetNPCFlag(LONG a)
{
	if (a > (FLAG_BYTES * 8))
		return FALSE;
	if (gFlagNPC[a / 8] & (1 << a % 8))
		return TRUE;
	else
		return FALSE;
}
