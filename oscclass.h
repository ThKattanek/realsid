//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: oscclass.cpp                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 31.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef OSCCLASS_H
#define OSCCLASS_H

#include <waves.h>

class OSCClass
{
public:
    OSCClass();
    ~OSCClass();
    void Reset(void);
    void OneCycle(void);
    void SetFrequenz(unsigned short frequenz);
    void SetControlBits(unsigned char ctrlbits);
    void SetPulesCompare(unsigned short pulsecompare);
    unsigned short GetOutput(void);

private:
    unsigned long   FrequenzCounter;
    unsigned long   FrequenzCounterOld;
    unsigned long   ShiftRegister;
    unsigned long   Bit0;
    unsigned short  FrequenzAdd;
    unsigned short  PulseCompare;
    unsigned char   WaveForm;
    bool            TestBit;
};

#endif // OSCCLASS_H
