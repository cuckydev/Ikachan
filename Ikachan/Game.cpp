#include "Game.h"
#include "Generic.h"
#include "Item.h"
#include "Draw.h"
#include "Flags.h"
#include "EventScript.h"
#include "Opening.h"
#include "Map.h"
#include "Player.h"
#include "NpChar.h"
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
	GAMEMODE_EDITOR = 2,
	GAMEMODE_INTRO = 5,
	GAMEMODE_LOAD = 6,
};

BOOL Game(HWND hWnd)
{
	TCHAR path[MAX_PATH];
	
	OPENING opening;
	FADE fade;
	ITEMS items;
	EVENT_SCR event_scr;
	MAP map;
	NPCHAR npc[MAX_NPCS];
	FRAME frame;
	DWORD tick;
	
	//Load generic data
	LoadGenericData();
	
	//Initalize game
	InitFlags();
	InitItem(&items);
	InitEventScript(&event_scr);
	InitMyChar();
	InitNpChar(npc);
	InitTextObject(NULL);
	
	//Initialize fade
	fade.mode = FADE_MODE_NONE;
	fade.mask = FALSE;
	
	//Initialize frame
	frame.x = gMC.x - (SURFACE_WIDTH << 9);
	frame.y = gMC.y - (SURFACE_HEIGHT << 9);
	
	//Load fade surface here for some reason
	MakeSurface_File("Pbm\\Fade.pbm", SURFACE_ID_FADE);
	
	//Start game (start in opening)
	char mode = GAMEMODE_OPENING;
	InitOpening(&opening);
	char next_mode = mode;
	
	//Opening
	while (mode == GAMEMODE_OPENING)
	{
		//Start frame
		tick = GetTickCount();
		GetTrg();
		CortBox(&grcFull, 0x000000);
		
		//Run opening
		if (gKeyTrg & KEY_Z)
			fade.mode = FADE_MODE_FADEOUT;
		PutOpening(&opening);
		if (ProcFade(&fade, &frame) == TRUE)
			mode = GAMEMODE_LOAD;
		
		//End frame
		if (!Flip_SystemTask(hWnd))
			return TRUE;
	}
	
	//Load map event and data
	sprintf(path, "%s\\%s", gModulePath, "Event.ptx");
	if (!ReadEventScript(path, &event_scr))
		return TRUE;
	DebugPutText((LPCTSTR)event_scr.data);
	
	sprintf(path, "%s\\%s", gModulePath, "Pbm\\Map1.pbm");
	if (!LoadMapData(path, &map))
		return TRUE;
	
	//Run load script
	event_scr.mode = 1;
	event_scr.x1C = 4;
	event_scr.event_no = 1;
	
	//Reset frame state
	frame.mode = FRAME_MODE_MYCHAR;
	frame.npc = 0;
	
	//Load screen
	next_mode = GAMEMODE_INTRO;
	while (mode == GAMEMODE_LOAD)
	{
		//Start frame
		tick = GetTickCount();
		GetTrg();
		CortBox(&grcFull, 0x000000);
		
		//Run load script
		if (EventScriptProc(&event_scr, &items, npc, &map, &fade, &frame) == 2)
		{
			event_scr.wait = 0;
			event_scr.mode = 3;
		}
		if (event_scr.mode == 0)
			mode = next_mode;
		PutMsgBox(&event_scr);
		
		//End frame
		if (!Flip_SystemTask(hWnd))
			return TRUE;
	}

	//Prepare for intro
	if (gKey & KEY_S)
		mode = GAMEMODE_EDITOR;
	if (mode == GAMEMODE_INTRO)
	{

	}
	return FALSE;
}
