//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: oscclass.cpp                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 28.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#include "oscclass.h"

OSCClass::OSCClass()
{
    FrequenzAdd = 0;
    WaveForm = 0;
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

    switch(WaveForm)
    {
    case 0:
        return 0;
        break;
    case 1:
        if(FrequenzCounterMSB) return (~FrequenzCounter>>11)&0xFFF;
        else return (FrequenzCounter>>11)&0xFFF;
        break;
    case 2:
        return FrequenzCounter>>12;
        break;
    default:
        return FrequenzCounter>>12;
        break;
    }
}
