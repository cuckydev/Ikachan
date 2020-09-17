#include "Item.h"
#include "Draw.h"
#include <string.h>

void InitItem(ITEMS *items)
{
	//Allocate items
	items->code = (char*)LocalAlloc(LPTR, MAX_ITEM * sizeof(char));

	//Reset item codes
	for (int i = 0; i < MAX_ITEM; i++)
		items->code[i] = i + 1;
}

void PutItem(ITEMS *items)
{
	//Draw background and frame
	PutBitmap3(&grcFull, 0, 0, &grcFull, SURFACE_ID_BACKUP);

	RECT rcFrame = { 0, 0, 176, 32 };
	PutBitmap3(&grcFull, (SURFACE_WIDTH - 176) / 2, (SURFACE_HEIGHT / 2) + 8, &rcFrame, SURFACE_ID_ITEMBOX);

	//Draw items
	RECT rcItem = { 0, 0, 0, 16 };
	for (int i = 0; i < MAX_ITEM; i++)
	{
		rcItem.left = (items->code[i] - 1) * 16;
		rcItem.right = rcItem.left + 16;
		PutBitmap3(&grcFull, (SURFACE_WIDTH - (16 * MAX_ITEM)) / 2 + (i * 16), (SURFACE_HEIGHT / 2) + 16, &rcItem, SURFACE_ID_ITEM);
	}

	//Draw cursor
	RECT rcCursor = {32, 0, 48, 16};
	PutBitmap3(&grcFull, (SURFACE_WIDTH - (16 * MAX_ITEM)) / 2 + (items->selected_item * 16), (SURFACE_HEIGHT / 2) + 16, &rcCursor, SURFACE_ID_CURSOR);
}