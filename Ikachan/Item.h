#pragma once
#include <windows.h>

#define MAX_ITEM 10

struct ITEMS
{
	CHAR *code;
	int selected_item;
};

void InitItem(ITEMS *items);
void PutItem(ITEMS *items);
