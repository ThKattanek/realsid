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
    void OneCycle(void);
    void SetFrequenz(unsigned short frequenz);
    void SetWaveForm(unsigned char waveform);
    unsigned short GetOutput(void);

private:
    unsigned long   FrequenzCounter;
    bool            FrequenzCounterMSB;
    unsigned short  FrequenzAdd;
    unsigned char   WaveForm;
};

#endif // OSCCLASS_H
