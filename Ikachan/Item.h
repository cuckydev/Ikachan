#pragma once
#include <windows.h>

#define MAX_ITEM 10

struct ITEM
{
	BYTE code;
};

struct ITEMS
{
	ITEM *item;
	int selected_item;
};

void InitItem(ITEMS *items);
void PutItem(ITEMS *items);
