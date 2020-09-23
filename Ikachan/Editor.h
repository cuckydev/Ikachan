#pragma once
#include "NpChar.h"
#include "Map.h"
#include <windows.h>

extern BOOLEAN gEditorMode;
extern short gEditorNPC;

void InitEditor();
void PutEditorCursor();
void PutEditorSelect(NPCHAR *npc, FRAME *frame);
void EditorProc(NPCHAR *npc);
void PutEditorNpcInfo(NPCHAR *npc);
BOOL LoadNpChar(NPCHAR *npc);
BOOL SaveNpChar(NPCHAR *npc);
