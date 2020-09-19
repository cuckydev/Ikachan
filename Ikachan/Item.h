#pragma once
#include <windows.h>

#define MAX_ITEMS (10)

struct ITEMS
{
	char *code;
	char selected_item;
};

struct EVENT_SCR; //forward declaration

void InitItem(ITEMS *items);
void PutItem(ITEMS *items);
void MoveItem(ITEMS *items, EVENT_SCR *event_scr);
BOOL CheckItem(ITEMS *items, char code);
BOOL AddItemData(ITEMS *items, char code);
BOOL SubItemData(ITEMS *items, char code);
