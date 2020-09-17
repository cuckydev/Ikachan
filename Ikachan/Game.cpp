#include "Game.h"
#include "Generic.h"
#include "Item.h"
#include "Draw.h"
#include "Flags.h"
#include "EventScript.h"
#include "Opening.h"
#include "EventFade.h"
#include "Map.h"
#include "Player.h"
#include <stdio.h>

DWORD gKeyTrg, gMouseTrg, gMouseTrg2;

void GetTrg()
{
	static DWORD key_prev, mouse_prev;

	//Get pressed key
	gKeyTrg = (key_prev ^ gKey) & gKey;
	key_prev = gKey;

	//Get pressed mouse
	gMouseTrg = (mouse_prev ^ gMouse) & gMouse;
	mouse_prev = gMouse;

	//Get some mouse thing
	static int timer;
	if (gMouse)
		timer++;
	else
		timer = 0;

	gMouseTrg2 = 0;
	if (timer > 20 || timer == 1)
		gMouseTrg2 = gMouse;
}

DWORD CountFramePerSecound()
{
	unsigned long current_tick;
	static BOOL first = TRUE;
	static unsigned long max_count;
	static unsigned long count;
	static unsigned long wait;

	if (first)
	{
		wait = GetTickCount();
		first = FALSE;
	}

	current_tick = GetTickCount();
	++count;

	if (wait + 1000 <= current_tick)
	{
		wait += 1000;
		max_count = count;
		count = 0;
	}

	return max_count;
}

enum GAMEMODE
{
	GAMEMODE_OPENING,
	GAMEMODE_LOAD = 6,
};

BOOL Game(HWND hWnd)
{
	TCHAR path[MAX_PATH];
	
	OPENING opening;
	FADE1 fade;
	ITEMS items;
	EVENT_SCR event_scr;
	MAP map;
	DWORD tick; //TODO: probably a piyopiyo instance
	
	//Load generic data
	LoadGenericData();

	//Initalize game
	InitFlags();
	InitItem(&items);
	InitEventScript(&event_scr);
	InitMyChar();
	InitTextObject(NULL);

	fade.mode = FM_NONE;
	fade.mask = FALSE;

	//Load fade surface here for some reason
	MakeSurface_File("Pbm\\Fade.pbm", SURFACE_ID_FADE);

	//Start game (start in opening)
	char mode = GAMEMODE_OPENING;
	InitOpening(&opening);

	//Opening
	while (mode == GAMEMODE_OPENING)
	{
		tick = GetTickCount();
		GetTrg();
		CortBox(&grcFull, 0x000000);
		if (gKeyTrg & KEY_Z)
			fade.mode = FM_FADEOUT;
		PutOpening(&opening);
		if (ProcFade(&fade) == TRUE)
			mode = GAMEMODE_LOAD;
		if (!Flip_SystemTask(hWnd))
			return TRUE;
	}

	//Load map event and data
	sprintf(path, "%s\\%s", gModulePath, "Event.ptx");
	if (!ReadEventScript(path, &event_scr))
		return TRUE;

	sprintf(path, "%s\\%s", gModulePath, "Pbm\\Map1.pbm");
	if (!LoadMapData(path, &map))
		return TRUE;

	while (1)
	{
		tick = GetTickCount();
		GetTrg();
		CortBox(&grcFull, 0x000000);
		ActMyChar(TRUE);
		PutMyChar();
		if (!Flip_SystemTask(hWnd))
			return TRUE;
	}
	return FALSE;
}
