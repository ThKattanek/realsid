//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sidclass.h                            //
//						//
// Geistiges Eigentum von Thorsten Kattanek     //
//						//
// Letzte Änderung am 27.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef SIDCLASS_H
#define SIDCLASS_H

#include <oscclass.h>

class SIDClass
{
public:
    SIDClass(int samplerate,int puffersize);
    ~SIDClass();
    void ResetSoundPufferPos(void);
    void OneCycle(void);
    float* GetSoundPuffer(void);
    int GetSoundPufferPos(void);
    void WriteIO(unsigned short adresse, unsigned char wert);

private:
    void Reset(void);

    float       SampleRate;             // SampleRate der SoundAusgabe
    int         PufferSize;             // Puffergröße
    float       *SoundPuffer;           // Soundpuffer
    int         SoundPufferPos;         // Zeigt auf die Position innerhalb des Soundpuffer

    double      FreqConvCounter;	// interner Counter für Fast-Resampling ohne interpolieren
    double      FreqConvAddWert;        // Wert der bei jedem Zyklus zum FreqConvCounter hinzu addiert wird

    OSCClass    *osc[3];                // Alles 3 Oszillatoren

    /// Alle IO Register ///
    unsigned char Freq0Lo;
    unsigned char Freq0Hi;
    unsigned char Freq1Lo;
    unsigned char Freq1Hi;
    unsigned char Freq2Lo;
    unsigned char Freq2Hi;
    unsigned char Ctrl0;
    unsigned char Ctrl1;
    unsigned char Ctrl2;
};

#endif // SIDCLASS_H
