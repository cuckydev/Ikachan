#include "PiyoPiyo.h"
#include "Sound.h"

BOOL InitPiyoPiyo()
{
	if (!InitSoundObject("BASS1", 472))
		return FALSE;
	InitSoundObject("BASS1", 473);
	InitSoundObject("BASS2", 474);
	InitSoundObject("BASS2", 475);
	InitSoundObject("SNARE1", 476);
	InitSoundObject("SNARE1", 477);
	InitSoundObject("SNARE1", 478);
	InitSoundObject("SNARE1", 479);
	InitSoundObject("HAT1", 480);
	InitSoundObject("HAT1", 481);
	InitSoundObject("HAT2", 482);
	InitSoundObject("HAT2", 483);
	InitSoundObject("SYMBAL1", 484);
	InitSoundObject("SYMBAL1", 485);
	InitSoundObject("SYMBAL1", 486);
	InitSoundObject("SYMBAL1", 487);
	InitSoundObject("SYMBAL1", 488);
	InitSoundObject("SYMBAL1", 489);
	InitSoundObject("SYMBAL1", 490);
	InitSoundObject("SYMBAL1", 491);
	InitSoundObject("SYMBAL1", 492);
	InitSoundObject("SYMBAL1", 493);
	InitSoundObject("SYMBAL1", 494);
	InitSoundObject("SYMBAL1", 495);
	return TRUE;
}

void EndPiyoPiyo()
{

}