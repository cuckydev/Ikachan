#include "Sound.h"
#include "System.h"

#define DIRECTSOUND_VERSION 0x500
#include <dsound.h>

#include <stdio.h>

//DirectSound objects
LPDIRECTSOUND       lpDS;
LPDIRECTSOUNDBUFFER lpPRIMARYBUFFER;

//DirectSound buffers
#define SE_MAX 512
LPDIRECTSOUNDBUFFER lpSECONDARYBUFFER[SE_MAX];

//DirectSound interface
BOOL InitDirectSound(HWND hwnd)
{
	//Create DirectSound object
	if (DirectSoundCreate(NULL, &lpDS, NULL) != DS_OK)
		return FALSE;
	lpDS->SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE);

	//Create primary sound buffer
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	lpDS->CreateSoundBuffer(&dsbd, &lpPRIMARYBUFFER, NULL);

	//Clear secondary sound buffers
	for (int i = 0; i < SE_MAX; i++)
		lpSECONDARYBUFFER[i] = NULL;
	return TRUE;
}

void EndDirectSound()
{
	//Release secondary sound buffers
	for (int i = 0; i < 8; i++) //Should be SE_MAX
	{
		if (lpSECONDARYBUFFER[i] != NULL)
		{
			lpSECONDARYBUFFER[i]->Release();
			lpSECONDARYBUFFER[i] = NULL;
		}
	}

	//Release primary sound buffer
	if (lpPRIMARYBUFFER != NULL)
	{
		lpPRIMARYBUFFER->Release();
		lpPRIMARYBUFFER = NULL;
	}

	//Release DirectSound object
	if (lpDS != NULL)
	{
		lpDS->Release();
		lpDS = NULL;
	}
}

//Sound object creation and release
void ReleaseSoundObject(int no)
{
	if (lpSECONDARYBUFFER[no] != NULL)
	{
		lpSECONDARYBUFFER[no]->Release();
		lpSECONDARYBUFFER[no] = NULL;
	}
}

BOOL InitSoundObject(LPCSTR resname, int no)
{
	//Find and lock resource
	HRSRC hrscr;
	if ((hrscr = FindResource(NULL, resname, "WAVE")) == NULL)
		return FALSE;
	DWORD *lpdword = (DWORD*)LockResource(LoadResource(NULL, hrscr));

	//Create sound buffer
	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_STICKYFOCUS | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = *(DWORD*)((BYTE*)lpdword + 0x36);
	dsbd.lpwfxFormat = (LPWAVEFORMATEX)(lpdword + 5);
	if (lpDS->CreateSoundBuffer(&dsbd, &lpSECONDARYBUFFER[no], NULL) != DS_OK)
		return FALSE;

	//Lock sound buffer
	LPVOID lpbuf1, lpbuf2;
	DWORD dwbuf1, dwbuf2;
	lpSECONDARYBUFFER[no]->Lock(0, *(DWORD*)((BYTE*)lpdword + 0x36),
		&lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0);

	//Copy data
	memcpy(lpbuf1, (BYTE*)lpdword + 0x3a, dwbuf1);
	if (dwbuf2 != 0)
		memcpy(lpbuf2, (BYTE*)lpdword + 0x3a + dwbuf1, dwbuf2);
	
	//Unlock sound buffer
	lpSECONDARYBUFFER[no]->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2);
	return TRUE;
}

void PlaySoundObject(int no, int mode)
{
	if (lpSECONDARYBUFFER[no] != NULL)
	{
		switch (mode)
		{
			case SOUND_MODE_STOP: //Stop
				lpSECONDARYBUFFER[no]->Stop();
				break;

			case SOUND_MODE_PLAY: //Play
				lpSECONDARYBUFFER[no]->Stop();
				lpSECONDARYBUFFER[no]->SetCurrentPosition(0);
				lpSECONDARYBUFFER[no]->Play(0, 0, 0);
				break;

			case SOUND_MODE_PLAY_LOOP: //Play looped
				lpSECONDARYBUFFER[no]->Play(0, 0, DSBPLAY_LOOPING);
				break;
		}
	}
}

void ChangeSoundFrequency(int no, DWORD rate) //100 is MIN, 9999 is MAX, and 2195 is normal
{
	if (lpSECONDARYBUFFER[no] != NULL)
		lpSECONDARYBUFFER[no]->SetFrequency((rate * 10) + 100);
}

void ChangeSoundVolume(int no, long volume) //300 is MAX and 300 is normal
{
	if (lpSECONDARYBUFFER[no] != NULL)
		lpSECONDARYBUFFER[no]->SetVolume((volume - 300) * 8);
}

void ChangeSoundPan(int no, long pan) //512 is MAX and 256 is normal
{
	if (lpSECONDARYBUFFER[no] != NULL)
		lpSECONDARYBUFFER[no]->SetPan((pan - 256) * 10);
}