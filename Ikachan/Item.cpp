#include "Item.h"
#include "Draw.h"
#include "Player.h"
#include "EventScript.h"
#include "Sound.h"
#include "System.h"
#include <string.h>

BYTE item_equip[12] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x02, 0x00, 0x00, 0x08 };

//Item inventory initialization and inventory drawing
void InitItem(ITEMS *items)
{
	//Allocate items
	items->code = (char*)LocalAlloc(LPTR, MAX_ITEMS * sizeof(char));

	//Reset item codes
	for (int i = 0; i < MAX_ITEMS; i++)
		items->code[i] = 0;
}

void PutItem(ITEMS *items)
{
	RECT rcFrame = { 0, 0, 176, 32 };
	RECT rcItem = { 0, 0, 0, 16 };
	RECT rcCursor = {32, 0, 48, 16};
	
	//Draw background and frame
	PutBitmap3(&grcFull, 0, 0, &grcFull, SURFACE_ID_BACKUP);
	PutBitmap3(&grcFull, (SURFACE_WIDTH - 176) / 2, (SURFACE_HEIGHT / 2) + 8, &rcFrame, SURFACE_ID_ITEMBOX);

	//Draw items
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		rcItem.left = (items->code[i] - 1) * 16;
		rcItem.right = rcItem.left + 16;
		PutBitmap3(&grcFull, (SURFACE_WIDTH - (16 * MAX_ITEMS)) / 2 + (i * 16), (SURFACE_HEIGHT / 2) + 16, &rcItem, SURFACE_ID_ITEM);
	}

	//Draw cursor
	PutBitmap3(&grcFull, (SURFACE_WIDTH - (16 * MAX_ITEMS)) / 2 + (items->selected_item * 16), (SURFACE_HEIGHT / 2) + 16, &rcCursor, SURFACE_ID_CURSOR);
}

void MoveItem(ITEMS *items, EVENT_SCR *event_scr)
{
	//Move selection with left and right
	if (gKeyTrg & KEY_LEFT)
	{
		PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
		if (--items->selected_item < 0)
			items->selected_item = MAX_ITEMS - 1;
	}
	if (gKeyTrg & KEY_RIGHT)
	{
		PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
		if (++items->selected_item >= MAX_ITEMS)
			items->selected_item = 0;
	}
	
	//Display item description when Z is pressed
	if (gKeyTrg & KEY_Z)
	{
		PlaySoundObject(SOUND_ID_DASH, SOUND_MODE_PLAY);
		char code = items->code[items->selected_item];
		if (code != 0)
		{
			event_scr->event_no = 2000 + code;
			event_scr->mode = 1;
		}
	}
}

//Check, give, and remove items
BOOL CheckItem(ITEMS *items, char code)
{
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		//Check if this is the requested item
		if (items->code[i] == code)
			return TRUE;
	}
	return FALSE;
}

BOOL AddItemData(ITEMS *items, char code)
{
	//Check if we already have this item
	if (CheckItem(items, code) == FALSE)
	{
		for (int i = 0; i < MAX_ITEMS; i++)
		{
			//Check if this is a free item slot
			if (items->code[i] == 0)
			{
				//Fill item slot and equip
				items->code[i] = code;
				gMC.equip |= item_equip[code];
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL SubItemData(ITEMS *items, char code)
{
	//Check if we already have this item
	if (CheckItem(items, code) == TRUE)
	{
		for (int i = 0; i < MAX_ITEMS; i++)
		{
			//Check if this is the requested item to remove
			if (items->code[i] == code)
			{
				//Remove item from inventory and dequip
				items->code[i] = 0;
				gMC.equip &= ~item_equip[code];
				return TRUE;
			}
		}
	}
	return FALSE;
}
