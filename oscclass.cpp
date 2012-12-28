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

void OSCClass::Reset()
{
    FrequenzCounter = 0;
    ShiftRegister = 0x7FFFF8;
    FrequenzAdd = 0;
    PulseCompare = 0;
    WaveForm = 0;
}

void OSCClass::OneCycle()
{
    FrequenzCounterOld = FrequenzCounter;
    FrequenzCounter += FrequenzAdd;
    FrequenzCounter &= 0xFFFFFF;

    if(!(FrequenzCounterOld&0x080000)&&(FrequenzCounter&0x080000))
    {
        TMP0 = ((ShiftRegister>>22)^(ShiftRegister>>17))&0x01;
        ShiftRegister <<= 1;
        ShiftRegister &= 0x7FFFFF;
        ShiftRegister |= TMP0;
    }
}

void OSCClass::SetFrequenz(unsigned short frequenz)
{
    FrequenzAdd = frequenz;
}

void OSCClass::SetControlBits(unsigned char ctrlbits)
{
    WaveForm = (ctrlbits>>4);
}

void OSCClass::SetPulesCompare(unsigned short pulsecompare)
{
    PulseCompare = pulsecompare & 0xFFF;
}

unsigned short OSCClass::GetOutput()
{
    FrequenzCounterMSB = FrequenzCounter >> 23;

    if(WaveForm == 0) return 0;

    switch(WaveForm)
    {
    case 0:
        return 0x000;
        break;

    /// Die 4 Grundwellenformen ///
    case 1: // Dreieck
        if(FrequenzCounterMSB) return (~FrequenzCounter>>11) & 0xFFF;
        else return (FrequenzCounter>>11) & 0xFFF;
        break;
    case 2: // Sägezahn
        return FrequenzCounter>>12;
        break;
    case 4: // Rechteck
        if(PulseCompare == 0xFFF) return 0xFFF;
        if((FrequenzCounter>>12) >= PulseCompare) return 0xFFF;
        else return 0x000;
        break;
    case 8: // Rauschen
        return ((ShiftRegister&0x400000)>>11)|((ShiftRegister&0x100000)>>10)|((ShiftRegister&0x010000)>>7)|((ShiftRegister&0x002000)>>5)|((ShiftRegister&0x000800)>>4)|((ShiftRegister&0x000080)>>1)|((ShiftRegister&0x000010)<<1)|((ShiftRegister&0x000004)<< 2);
        break;

    /// Mischwellenformen ///
    default:
        return 0;
        break;
    }
}
