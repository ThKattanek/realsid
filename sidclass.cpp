//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sidclass.cpp                          //
//						//
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek		//
//						//
// Letzte Änderung am 23.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#include "sidclass.h"

SIDClass::SIDClass(int samplerate,int puffersize)
{
    SampleRate = (float)samplerate;
    PufferSize = puffersize;

    FreqConvAddWert=((float)1.0)/((float)985248.0/SampleRate);  // PAL (17734472 / 18) NTSC (14318180 / 14)
    FreqConvCounter=0.0;

    SoundPuffer = new float[puffersize];

    Wert = 0.0f;
}

SIDClass::~SIDClass()
{
    delete SoundPuffer;
}

void SIDClass::ResetSoundPufferPos()
{
    SoundPufferPos = 0;
}

float* SIDClass::GetSoundPuffer()
{
    return SoundPuffer;
}

int SIDClass::GetSoundPufferPos()
{
    return SoundPufferPos;
}

void SIDClass::OneCycle()
{
    Wert += 0.00003f;
    if(Wert >= 0.5f) Wert-=0.5f;

    FreqConvCounter+=FreqConvAddWert;
    if(FreqConvCounter>=(double)1.0)
    {
        FreqConvCounter-=(double)1.0;
        SoundPuffer[SoundPufferPos++] = Wert-0.25f;
    }
}
