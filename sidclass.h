//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sidclass.h                            //
//						//
// Geistiges Eigentum von Thorsten Kattanek     //
//						//
// Letzte �nderung am 31.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef SIDCLASS_H
#define SIDCLASS_H

#include <oscclass.h>
#include <envclass.h>
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
    int             PufferSize;             // Puffergr��e
    float           *SoundPuffer;           // Soundpuffer
    int             SoundPufferPos;         // Zeigt auf die Position innerhalb des Soundpuffer

    double          FreqConvCounter;        // interner Counter f�r Fast-Resampling ohne interpolieren
    double          FreqConvAddWert;        // Wert der bei jedem Zyklus zum FreqConvCounter hinzu addiert wird

    SIDDumpClass    *IODump;                // Wird ben�tigt um SIDDump Files vom Emu64 zu laden (Diese Klasse wird im Emu64 gepflegt)
    OSCClass        *OSC[3];                // Alle 3 Oszillatoren
    ENVClass        *ENV[3];                // Alle 3 H�llkurvengeneratoren

    /// Alle IO Register ///
    unsigned char   IO[32];
    unsigned char   Freq0Lo;
    unsigned char   Freq0Hi;
    unsigned char   Freq1Lo;
    unsigned char   Freq1Hi;
    unsigned char   Freq2Lo;
    unsigned char   Freq2Hi;
    unsigned char   Puls0Lo;
    unsigned char   Puls0Hi;
    unsigned char   Puls1Lo;
    unsigned char   Puls1Hi;
    unsigned char   Puls2Lo;
    unsigned char   Puls2Hi;
    unsigned char   Ctrl0;
    unsigned char   Ctrl1;
    unsigned char   Ctrl2;
};

#endif // SIDCLASS_H
