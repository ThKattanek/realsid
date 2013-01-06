//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sidclass.cpp                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 03.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#include "sidclass.h"

SIDClass::SIDClass(int samplerate,int puffersize)
{
    SampleRate = (float)samplerate;
    PufferSize = puffersize;

    FreqConvAddWert=1.0f/((float)985250/SampleRate);  // PAL (17734472 / 18) NTSC (14318180 / 14)
    FreqConvCounter=0.0f;

    SoundPuffer = new float[puffersize];
    IODump = new SIDDumpClass(IO);

    for(int i=0;i<3;i++)
    {
        OSC[i] = new OSCClass();
        ENV[i] = new ENVClass();
    }

    OSC[0]->SetOSCSource(OSC[2]);
    OSC[1]->SetOSCSource(OSC[0]);
    OSC[2]->SetOSCSource(OSC[1]);

    OSC[0]->SetOSCDestination(OSC[1]);
    OSC[1]->SetOSCDestination(OSC[2]);
    OSC[2]->SetOSCDestination(OSC[0]);

    Filter = new FilterClass();

    Reset();
}

SIDClass::~SIDClass()
{
    delete SoundPuffer;
    delete IODump;

    for(int i=0;i<3;i++)
    {
        delete ENV[i];
        delete OSC[i];
    }

    delete Filter;
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
    OSC[0]->OneCycle();
    OSC[1]->OneCycle();
    OSC[2]->OneCycle();

    ENV[0]->OneCycle();
    ENV[1]->OneCycle();
    ENV[2]->OneCycle();

    Filter->OneCycle(OSC,ENV);

    FreqConvCounter += FreqConvAddWert;
    if(FreqConvCounter >= 1.0f)
    {
        FreqConvCounter-=(double)1.0;
        SoundPuffer[SoundPufferPos++] = float(Filter->GetOutput()>>3) / (float)0xFFFF;
    }

    /// SIDDump Klasse vom Emu64 für Testzwecke ///

    // Record //
    IODump->CycleTickCapture();

    // Play //
    if(IODump->CycleTickPlay()) WriteIO(IODump->RegOut,IODump->RegWertOut);
}

void SIDClass::Reset()
{
    Freq0Lo = 0;
    Freq0Hi = 0;
    Freq1Lo = 0;
    Freq1Hi = 0;
    Freq2Lo = 0;
    Freq2Hi = 0;
    Puls0Lo = 0;
    Puls0Hi = 0;
    Puls1Lo = 0;
    Puls1Hi = 0;
    Puls2Lo = 0;
    Puls2Hi = 0;
    Ctrl0 = 0;
    Ctrl1 = 0;
    Ctrl2 = 0;

    FilterFreqLo = 0;
    FilterFreqHi = 0;

    OSC[0]->Reset();
    OSC[1]->Reset();
    OSC[2]->Reset();

    ENV[0]->Reset();
    ENV[1]->Reset();
    ENV[2]->Reset();

    Filter->Reset();
}

void SIDClass::WriteIO(unsigned short adresse, unsigned char wert)
{
    IO[adresse & 31] = wert;

    switch(adresse & 0x1F)
    {
    case 0: // Oszillatorfrequenz LoByte Stimme 1
        Freq0Lo = wert;
        OSC[0]->SetFrequenz((Freq0Hi<<8)|Freq0Lo);
        break;
    case 1: // Oszillatorfrequenz HiByte Stimme 1
        Freq0Hi = wert;
        OSC[0]->SetFrequenz((Freq0Hi<<8)|Freq0Lo);
        break;
    case 2: // Pulsweite LoByte Stimme 1
        Puls0Lo = wert;
        OSC[0]->SetPulesCompare((Puls0Hi<<8)|Puls0Lo);
        break;
    case 3: // Pulsweite HiByte Stimme 1
        Puls0Hi = wert;
        OSC[0]->SetPulesCompare((Puls0Hi<<8)|Puls0Lo);
        break;
    case 4: // Steuerregister Stimme 1
        Ctrl0 = wert;
        OSC[0]->SetControlBits(Ctrl0);
        ENV[0]->SetKeyBit(wert & 0x01);
        break;
    case 5: // Attack & Decay Stimme 1
        ENV[0]->SetAttackDecay(wert);
        break;
    case 6: // Sustain & Release Stimme 1
        ENV[0]->SetSustainRelease(wert);
        break;
    case 7: // Oszillatorfrequenz LoByte Stimme 2
        Freq1Lo = wert;
        OSC[1]->SetFrequenz((Freq1Hi<<8)|Freq1Lo);
        break;
    case 8: // Oszillatorfrequenz HiByte Stimme 2
        Freq1Hi = wert;
        OSC[1]->SetFrequenz((Freq1Hi<<8)|Freq1Lo);
        break;
    case 9: // Pulsweite LoByte Stimme 2
        Puls1Lo = wert;
        OSC[1]->SetPulesCompare((Puls1Hi<<8)|Puls1Lo);
        break;
    case 10: // Pulsweite HiByte Stimme 2
        Puls1Hi = wert;
        OSC[1]->SetPulesCompare((Puls1Hi<<8)|Puls1Lo);
        break;
    case 11: // Steuerregister Stimme 2
        Ctrl1 = wert;
        OSC[1]->SetControlBits(Ctrl1);
        ENV[1]->SetKeyBit(wert & 0x01);
        break;
    case 12: // Attack & Decay Stimme 2
        ENV[1]->SetAttackDecay(wert);
        break;
    case 13: // Sustain & Release Stimme 2
        ENV[1]->SetSustainRelease(wert);
        break;
    case 14: // Oszillatorfrequenz LoByte Stimme 3
        Freq2Lo = wert;
        OSC[2]->SetFrequenz((Freq2Hi<<8)|Freq2Lo);
        break;
    case 15: // Oszillatorfrequenz HiByte Stimme 3
        Freq2Hi = wert;
        OSC[2]->SetFrequenz((Freq2Hi<<8)|Freq2Lo);
        break;
    case 16: // Pulsweite LoByte Stimme 3
        Puls2Lo = wert;
        OSC[2]->SetPulesCompare((Puls2Hi<<8)|Puls2Lo);
        break;
    case 17: // Pulsweite HiByte Stimme 3
        Puls2Hi = wert;
        OSC[2]->SetPulesCompare((Puls2Hi<<8)|Puls2Lo);
        break;
    case 18: // Steuerregister Stimme 3
        Ctrl2 = wert;
        OSC[2]->SetControlBits(Ctrl2);
        ENV[2]->SetKeyBit(wert & 0x01);
        break;
    case 19: // Attack & Decay Stimme 2
        ENV[2]->SetAttackDecay(wert);
        break;
    case 20: // Sustain & Release Stimme 2
        ENV[2]->SetSustainRelease(wert);
        break;
    case 21: // Filterfrequenz LoByte
        FilterFreqLo = wert;
        Filter->SetFrequenz((FilterFreqHi<<3) | (FilterFreqLo & 0x07));
        break;
    case 22: // Filterfrequenz HByte
        FilterFreqHi = wert;
        Filter->SetFrequenz((FilterFreqHi<<3) | (FilterFreqLo & 0x07));
        break;
    case 23: // FilterControl1
        Filter->SetControl1(wert);
        break;
    case 24: // FilterControl2
        Filter->SetControl2(wert);
        break;

    default:
        break;
    }
}

bool SIDClass::OpenSIDDump(char *filename)
{
    return IODump->LoadDump(filename);
}

void SIDClass::PlaySIDDump()
{
    IODump->PlayDump();
}

void SIDClass::StopSIDDump()
{
    IODump->StopDump();
}
