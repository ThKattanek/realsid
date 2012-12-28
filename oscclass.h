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

#ifndef OSCCLASS_H
#define OSCCLASS_H

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

    bool MultiWave;

private:
    unsigned long   FrequenzCounter;
    unsigned long   FrequenzCounterOld;
    unsigned long   ShiftRegister;
    unsigned long   TMP0;
    bool            FrequenzCounterMSB;
    unsigned short  FrequenzAdd;
    unsigned short  PulseCompare;
    unsigned char   WaveForm;
};

#endif // OSCCLASS_H
