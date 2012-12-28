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
#include <siddump.h>

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
    bool OpenSIDDump(char *filename);
    void PlaySIDDump(void);
    void StopSIDDump(void);

private:
    void Reset(void);

    float           SampleRate;             // SampleRate der SoundAusgabe
    int             PufferSize;             // Puffergröße
    float           *SoundPuffer;           // Soundpuffer
    int             SoundPufferPos;         // Zeigt auf die Position innerhalb des Soundpuffer

    double          FreqConvCounter;        // interner Counter für Fast-Resampling ohne interpolieren
    double          FreqConvAddWert;        // Wert der bei jedem Zyklus zum FreqConvCounter hinzu addiert wird

    SIDDumpClass    *IODump;               // Wird benötigt um SIDDump Files vom Emu64 zu laden (Diese Klasse wird im Emu64 gepflegt)
    OSCClass        *OSC[3];                // Alles 3 Oszillatoren

    /// Alle IO Register ///
    unsigned char   IO[32];
    unsigned char   Freq0Lo;
    unsigned char   Freq0Hi;
    unsigned char   Freq1Lo;
    unsigned char   Freq1Hi;
    unsigned char   Freq2Lo;
    unsigned char   Freq2Hi;
    unsigned char   Ctrl0;
    unsigned char   Ctrl1;
    unsigned char   Ctrl2;
};

#endif // SIDCLASS_H
