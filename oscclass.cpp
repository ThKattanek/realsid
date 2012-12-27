//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: oscclass.cpp                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 27.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#include "oscclass.h"

OSCClass::OSCClass()
{
    FrequenzAdd = 0;
    WaveForm = 2;
}

OSCClass::~OSCClass()
{
}

void OSCClass::OneCycle()
{
    FrequenzCounter += FrequenzAdd;
    FrequenzCounter &= 0xFFFFFF;
    FrequenzCounterMSB = FrequenzCounter >> 23;
}

void OSCClass::SetFrequenz(unsigned short frequenz)
{
    FrequenzAdd = frequenz;
}

void OSCClass::SetWaveForm(unsigned char waveform)
{
    WaveForm = waveform;
}

unsigned short OSCClass::GetOutput()
{
    if(WaveForm == 0) return 0;

    unsigned short wave = 0x0000;

    if(WaveForm & 1)    // Dreieck
    {
        if(FrequenzCounterMSB) wave |= (~FrequenzCounter>>11)&0xFFF;
        else wave |= (FrequenzCounter>>11)&0xFFF;
    }

    if(WaveForm & 2)    // Sägezahn
    {
        wave |= FrequenzCounter>>12;
    }

    return wave;
}
