#include "Game.h"
#include "Generic.h"
#include "Item.h"
#include "Draw.h"

BOOL Game(HWND hWnd)
{
	//Load generic data
	LoadGenericData();

	//Initalize game
	ITEMS items;
	InitItem(&items);

	while (1)
	{
		CortBox(&grcFull, 0x000000);
		PutItem(&items);
		if (!Flip_SystemTask(hWnd))
			break;
	}
	return FALSE;
}
