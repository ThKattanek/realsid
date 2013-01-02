//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: oscclass.cpp                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 02.01.2013        	//
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
    void SetOSCSource(OSCClass *oscsource);
    void SetOSCDestination(OSCClass *oscdest);

private:
    unsigned short  GetDreieck(void);
    OSCClass        *OSCSource;
    OSCClass        *OSCDestination;
    unsigned long   FrequenzCounter;
    unsigned long   FrequenzCounterOld;
    bool            FrequenzCounterMsbRising;   /// (True wenn MSB von 0 nach 1 wechselt ansonsten False)
    unsigned long   ShiftRegister;
    unsigned long   Bit0;
    unsigned short  FrequenzAdd;
    unsigned short  PulseCompare;
    unsigned char   WaveForm;
    bool            TestBit;
    bool            RingBit;
    bool            SyncBit;
};

#endif // OSCCLASS_H
