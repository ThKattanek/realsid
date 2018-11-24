//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: envclass.cpp                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 02.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#include "envclass.h"

const unsigned int RateCounterPeriod[]={9,32,63,95,149,220,267,313,392,977,1954,3126,3907,11720,19532,31251};
const unsigned int SustainLevel[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

ENVClass::ENVClass()
{
    Reset();
}

ENVClass::~ENVClass()
{
}

void ENVClass::Reset()
{
    EnvCounter=0;
    Attack=0;
    Decay=0;
    Sustain=0;
    Release=0;
    KeyBit=false;
    RateCounter=0;
    State=RELEASE;
    RatePeriod=RateCounterPeriod[Release];
    HoldZero=true;
    ExponentialCounter=0;
    ExponentialCounterPeriod=1;
}

void ENVClass::OneCycle()
{
    if(++RateCounter & 0x8000) ++RateCounter &= 0x7FFF;
    if(RateCounter == RatePeriod)
    {
        RateCounter=0;

        if(State == ATTACK || ++ExponentialCounter == ExponentialCounterPeriod)
        {
            ExponentialCounter=0;

            if(!HoldZero)
            {
                switch(State)
                {
                case ATTACK:
                    ++EnvCounter &= 0xFF;
                    if(EnvCounter==0xFF)
                    {
                        State=DECAY_SUSTAIN;
                        RatePeriod=RateCounterPeriod[Decay];
                    }
                    break;

                case DECAY_SUSTAIN:
                    if(EnvCounter != SustainLevel[Sustain]) --EnvCounter;
                    break;

                case RELEASE:
                    --EnvCounter &= 0xFF;
                    break;
                }

                switch(EnvCounter)
                {
                case 0xFF:
                    ExponentialCounterPeriod=1;
                    break;

                case 0x5D:
                    ExponentialCounterPeriod=2;
                    break;

                case 0x36:
                    ExponentialCounterPeriod=4;
                    break;

                case 0x1A:
                    ExponentialCounterPeriod=8;
                    break;

                case 0x0E:
                    ExponentialCounterPeriod=16;
                    break;

                case 0x06:
                    ExponentialCounterPeriod=30;
                    break;

                case 0x00:
                    ExponentialCounterPeriod=1;
                    HoldZero=true;
                    break;
                }
            }
        }
    }
}

void ENVClass::SetKeyBit(bool wert)
{
    if (!KeyBit && wert)
    {
        State=ATTACK;
        RatePeriod=RateCounterPeriod[Attack];
        HoldZero=false;
    }
    else if (KeyBit && !wert)
    {
        State=RELEASE;
        RatePeriod=RateCounterPeriod[Release];
    }
    KeyBit = wert;
}

void ENVClass::SetAttackDecay(unsigned char wert)
{
    Attack=(wert>>4)&0x0F;
    Decay=wert&0x0F;
    if(State==ATTACK) RatePeriod=RateCounterPeriod[Attack];
    else if (State==DECAY_SUSTAIN) RatePeriod=RateCounterPeriod[Decay];
}

void ENVClass::SetSustainRelease(unsigned char wert)
{
    Sustain=(wert>>4)&0x0F;
    Release=wert&0x0F;
    if (State==RELEASE) RatePeriod=RateCounterPeriod[Release];
}

unsigned int ENVClass::GetOutput()
{
    return EnvCounter;
}
