#pragma once
#include <windows.h>

#define MAX_ITEMS (10)

struct ITEMS
{
	char *code;
	char selected_item;
};

void InitItem(ITEMS *items);
void PutItem(ITEMS *items);
BOOL CheckItem(ITEMS *items, char code);
BOOL AddItemData(ITEMS *items, char code);
BOOL SubItemData(ITEMS *items, char code);
