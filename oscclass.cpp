//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: oscclass.cpp                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 29.12.2012		//
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
    TestBit = false;
}

void OSCClass::OneCycle()
{
    if(!TestBit)
    {
        FrequenzCounterOld = FrequenzCounter;
        FrequenzCounter += FrequenzAdd;
        FrequenzCounter &= 0xFFFFFF;
    }

    if(!(FrequenzCounterOld&0x080000)&&(FrequenzCounter&0x080000))
    {
        Bit0 = ((ShiftRegister>>22)^(ShiftRegister>>17))&0x01;
        ShiftRegister = ((ShiftRegister<<1) & 0x7FFFFF) | Bit0;
    }
}

void OSCClass::SetFrequenz(unsigned short frequenz)
{
    FrequenzAdd = frequenz;
}

void OSCClass::SetControlBits(unsigned char ctrlbits)
{
    WaveForm = ctrlbits>>4;

    if(ctrlbits&0x08)
    {
        FrequenzCounter = 0;
        ShiftRegister = 0;
    }
    else if(TestBit) ShiftRegister = 0x7FFFF8;
    TestBit = (ctrlbits&0x08)>>3;
}

void OSCClass::SetPulesCompare(unsigned short pulsecompare)
{
    PulseCompare = pulsecompare & 0xFFF;
}

unsigned short OSCClass::GetOutput()
{
    if(WaveForm == 0) return 0;

    switch(WaveForm)
    {
    /// Die 4 Grundwellenformen ///
    case 1: // Dreieck
        return (FrequenzCounter & 0x800000)?(~FrequenzCounter>>11) & 0xFFF:(FrequenzCounter>>11) & 0xFFF;
    case 2: // Sägezahn
        return FrequenzCounter>>12;
    case 3: // Mischform Sägezahn/Dreieck
        return wave0[FrequenzCounter>>12]<<4;
    case 4: // Rechteck
        if(PulseCompare == 0xFFF) return 0xFFF;
        return ((FrequenzCounter>>12) >= PulseCompare)?0xFFF:0x000;
    case 5: // Mischform
        return ((wave1[((FrequenzCounter & 0x800000)?(~FrequenzCounter>>11) & 0xFFF:(FrequenzCounter>>11) & 0xFFF)>>1]<<4) & (((FrequenzCounter>>12) >= PulseCompare)?0xFFF:0x000));;
    case 6: // Mischform Sägezahn/Dreieck
        return ((wave2[FrequenzCounter>>12]<<4) & (((FrequenzCounter>>12) >= PulseCompare)?0xFFF:0x000));
    case 7: // Mischform Sägezahn/Dreieck
        return ((wave3[FrequenzCounter>>12]<<4) & (((FrequenzCounter>>12) >= PulseCompare)?0xFFF:0x000));
    case 8: // Rauschen
        return ((ShiftRegister&0x400000)>>11)|((ShiftRegister&0x100000)>>10)|((ShiftRegister&0x010000)>>7)|((ShiftRegister&0x002000)>>5)|((ShiftRegister&0x000800)>>4)|((ShiftRegister&0x000080)>>1)|((ShiftRegister&0x000010)<<1)|((ShiftRegister&0x000004)<< 2);
    /// Mischwellenformen ///
    default:
        return 0;
    }
}
