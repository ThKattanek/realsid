//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sidclass.cpp                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 27.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#include "sidclass.h"

SIDClass::SIDClass(int samplerate,int puffersize)
{
    SampleRate = (float)samplerate;
    PufferSize = puffersize;

    FreqConvAddWert=1.0f/((float)985248.6111/SampleRate);  // PAL (17734472 / 18) NTSC (14318180 / 14)
    FreqConvCounter=0.0f;

    SoundPuffer = new float[puffersize];
    IODump = new SIDDumpClass(IO);

    for(int i=0;i<3;i++)
        OSC[i] = new OSCClass();

    Reset();
}

SIDClass::~SIDClass()
{
    delete SoundPuffer;
    delete IODump;
    for(int i=0;i<3;i++)
        delete OSC[i];
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

    FreqConvCounter += FreqConvAddWert;
    if(FreqConvCounter >= 1.0f)
    {
        FreqConvCounter-=(double)1.0;
        SoundPuffer[SoundPufferPos++] = (float(OSC[0]->GetOutput())/(float)0xFFF)*2.0f-1.0f;
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
}

void SIDClass::WriteIO(unsigned short adresse, unsigned char wert)
{
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
    case 4: // Steuerregister Stimme 1
        Ctrl0 = wert;
        OSC[0]->SetWaveForm(Ctrl0>>4);
        break;
    case 7: // Oszillatorfrequenz LoByte Stimme 2
        Freq1Lo = wert;
        OSC[1]->SetFrequenz((Freq1Hi<<8)|Freq1Lo);
        break;
    case 8: // Oszillatorfrequenz HiByte Stimme 2
        Freq1Hi = wert;
        OSC[1]->SetFrequenz((Freq1Hi<<8)|Freq1Lo);
        break;
    case 11: // Steuerregister Stimme 2
        Ctrl1 = wert;
        OSC[1]->SetWaveForm(Ctrl1>>4);
        break;
    case 14: // Oszillatorfrequenz LoByte Stimme 3
        Freq2Lo = wert;
        OSC[2]->SetFrequenz((Freq2Hi<<8)|Freq2Lo);
        break;
    case 15: // Oszillatorfrequenz HiByte Stimme 3
        Freq2Hi = wert;
        OSC[2]->SetFrequenz((Freq2Hi<<8)|Freq2Lo);
        break;
    case 18: // Steuerregister Stimme 3
        Ctrl2 = wert;
        OSC[2]->SetWaveForm(Ctrl2>>4);
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
