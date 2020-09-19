#include "PixelScript.h"
#include "EventScript.h"
#include <stdio.h>

#define IS_COMMAND1(c1) (ptx->data[ptx->p_read] == '<' && ptx->data[ptx->p_read + 1] == (c1))
#define IS_COMMAND2(c1, c2) (ptx->data[ptx->p_read] == '<' && ptx->data[ptx->p_read + 1] == (c1) && ptx->data[ptx->p_read + 2] == (c2))

RECT rcPsIllust = { 0, 0, 320, 240 };
RECT rcPsEnd = { 0, 0, 48, 24 };
RECT rcPsLine = { 0, 0, SURFACE_WIDTH, 16 };

BOOL ReadPixelScript(PIX_SCR *ptx, LPCTSTR path)
{
	//Get filesize
	HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ptx->size = GetFileSize(hFile, NULL);
	CloseHandle(hFile);
	
	//Allocate data
	ptx->data = (LPSTR)LocalAlloc(LPTR, ptx->size + 1);
	
	//Open file
	FILE *fp = fopen(path, "rt");
	if (fp == NULL)
		return FALSE;
	
	//Read file
	fread(ptx->data, ptx->size, 1, fp);
	fclose(fp);
	return TRUE;
}

void LoadPixelScript(PIX_SCR *ptx, LPCTSTR path, char scale)
{
	//Initialize Pixel Script
	ptx->end = FALSE;
	ptx->p_read = 0;
	ptx->scale = scale;
	ptx->line = 0;
	for (int i = 0; i < MAX_PSLINES; i++)
	{
		ptx->ypos_line[i] = i * 24 * ptx->scale;
		CortBox2(&rcPsLine, 0x000000, SURFACE_ID_WORDS0 + i);
	}

	//Read script
	ReadPixelScript(ptx, path);
	DebugPutText(ptx->data);
}

int PixelScriptProc(PIX_SCR *ptx, PIYOPIYO_CONTROL *piyocont, BOOL ending)
{
	TCHAR c[44];
	
	//Draw illustration
	if (ending)
		PutBitmap3(&grcFull, (SURFACE_WIDTH - 320) / 2, (SURFACE_HEIGHT - 240) / 2, &rcPsIllust, SURFACE_ID_STAFF);
	else
		PutBitmap3(&grcFull, (SURFACE_WIDTH - 320) / 2, (SURFACE_HEIGHT - 240) / 2, &rcPsIllust, SURFACE_ID_MARUAME);
	
	if (ptx->end)
	{
		//If the game's ended, draw "END" text
		PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 24, (SURFACE_HEIGHT / 2) - 14, &rcPsEnd, SURFACE_ID_END);
		return 1;
	}
	else
	{
		//Process script and lines
		for (int i = 0; i < MAX_PSLINES; i++)
		{
			//Draw line
			PutBitmap3(&grcFull, 0, ptx->ypos_line[i] / ptx->scale, &rcPsLine, SURFACE_ID_WORDS0 + i);

			//Scroll line
			ptx->ypos_line[i] -= 2;
			if ((gKey & KEY_SPACE) || (gKey & KEY_Z))
				ptx->ypos_line[i] -= 6;

			//Check if line's scrolled over the top of the screen
			if (ptx->ypos_line[i] / ptx->scale < -24)
			{
				//Clear line and move to bottom of screen
				ptx->ypos_line[i] = SURFACE_HEIGHT * ptx->scale;
				CortBox2(&rcPsLine, 0x000000, SURFACE_ID_WORDS0 + i);

				//Read new line
				for (int j = 0; j < 40; j++)
				{
					if (ptx->data[ptx->p_read] == ' ')
					{
						//Type space
						c[j] = ' ';
						ptx->p_read++;
					}
					else if (ptx->data[ptx->p_read] & 0x80)
					{
						//Type Shift-JIS
						c[j] = ptx->data[ptx->p_read];
						c[++j] = ptx->data[ptx->p_read + 1];
						ptx->p_read += 2;
					}
					else if (IS_COMMAND2('p','f'))
					{
						//Fade PiyoPiyo
						ptx->p_read += 4;
						piyocont->mode = 4;
						j = 999;
					}
					else if (IS_COMMAND1('s'))
					{
						//Bring up "END" text
						ptx->end = TRUE;
					}
					else if (ptx->data[ptx->p_read] >= 'A' && ptx->data[ptx->p_read] <= 'z')
					{
						//English alphabet
						c[j] = ptx->data[ptx->p_read++];
					}
					else if (ptx->data[ptx->p_read] >= '0' && ptx->data[ptx->p_read] <= '9')
					{
						//English numerals
						c[j] = ptx->data[ptx->p_read++];
					}
					else if (ptx->data[ptx->p_read] == '.' || ptx->data[ptx->p_read] == '/' || ptx->data[ptx->p_read] == ',')
					{
						//Other specific characters
						c[j] = ptx->data[ptx->p_read++];
					}
					else if (IS_COMMAND1('e'))
					{
						//Start gameplay
						return 1;
					}
					else if (ptx->data[ptx->p_read] == '+')
					{
						//Print text onto line
						c[j] = 0;
						PutText2(8, 1, c, 0xFF0000, SURFACE_ID_WORDS0 + ptx->line);
						PutText2(8, 0, c, 0xFF8800, SURFACE_ID_WORDS0 + ptx->line);
						if (++ptx->line >= MAX_PSLINES)
							ptx->line = 0;
						ptx->p_read += 2;
						j = 999;
					}
					else
					{
						ptx->p_read++;
					}
				}
			}
		}

		//If the game's ended, draw "END" text
		if (ptx->end)
			PutBitmap3(&grcFull, (SURFACE_WIDTH / 2) - 24, (SURFACE_HEIGHT / 2) - 14, &rcPsEnd, SURFACE_ID_END);
		return 0;
	}
}

void EndPixelScript(PIX_SCR *ptx)
{
	//Release data
	LocalFree((HLOCAL)ptx->data);
}