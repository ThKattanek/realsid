//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sidclass.h                            //
//						//
// Dieser Sourcecode ist Copyright gesch�tzt!   //
// Geistiges Eigentum von Th.Kattanek		//
//						//
// Letzte �nderung am 23.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef SIDCLASS_H
#define SIDCLASS_H

class SIDClass
{
public:
    SIDClass(int samplerate,int puffersize);
    ~SIDClass();
    void ResetSoundPufferPos(void);
    void OneCycle(void);
    float* GetSoundPuffer(void);
    int GetSoundPufferPos(void);

    float       SampleRate;             // SampleRate der SoundAusgabe
    int         PufferSize;             // Puffergr��e
    float       *SoundPuffer;           // Soundpuffer
    int         SoundPufferPos;         // Zeigt auf die Position innerhalb des Soundpuffer

    double      FreqConvCounter;	// interner Counter f�r Fast-Resampling ohne interpolieren
    double      FreqConvAddWert;        // Wert der bei jedem Zyklus zum FreqConvCounter hinzu addiert wird

    float       Wert;
};

#endif // SIDCLASS_H
