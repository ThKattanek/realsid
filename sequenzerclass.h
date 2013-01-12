//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sequenzerclass.h                      //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 12.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef SEQUENZERCLASS_H
#define SEQUENZERCLASS_H

#define PAL_TAKT 985250         /// C64 Taktfrequenz für PAL Geräte (985248 /  50 = 19705)
#define SID_ANZAHL 1            /// Anzahl der anzusprechenden SID's

/// Alle Notenfrequenzen C0 - H7 (8 Oktaven)
const float NotenFrequenzen[96] =
{   16.4f,17.3f,18.4f,19.4f,20.6f,21.8f,23.1f,24.5f,26.0f,27.5f,29.1f,30.9f,
    32.7f,34.6f,36.7f,38.9f,41.2f,43.7f,46.2f,49.0f,51.9f,55.0f,58.3f,61.7f,
    65.4f,69.3f,73.4f,77.8f,82.4f,87.3f,92.5f,98.0f,103.8f,110.0f,116.5f,123.5f,
    130.8f,138.6f,146.8f,155.6f,164.8f,174.6f,185.0f,196.0f,207.7f,220.0f,233.1f,246.9f,
    261.6f,277.2f,293.7f,311.1f,329.6f,349.2f,370.0f,392.0f,415.3f,440.0f,466.2f,493.9f,
    523.3f,554.4f,587.3f,622.3f,659.3f,698.5f,740.0f,784.0f,830.6f,880.0f,932.3f,987.8f,
    1046.5f,1108.7f,1174.7f,1244.5f,1318.5f,1396.9f,1480.0f,1568.0f,1661.2f,1760.0f,1864.7f,1975.5f,
    2093.0f,2217.5f,2349.3f,2489.0f,2637.0f,2793.8f,2960.0f,3136.0f,3322.4f,3520.0f,3729.3f,3729.3
};

const int AttackTime[16] =
{
    2*985.2,8*985.2,16*985.2,24*985.2,38*985.2,56*985.2,68*985.2,80*985.2,100*985.2,250*985.2,500*985.2,800*985.2,1000*985.2,3000*985.2,5000*985.2,8000*985.2
};

const int DecayTime[16] =
{
    4*2*985.2,3*8*985.2,3*16*985.2,3*24*985.2,3*38*985.2,3*56*985.2,3*68*985.2,3*80*985.2,3*100*985.2,3*250*985.2,3*500*985.2,3*800*985.2,3*1000*985.2,3*3000*985.2,3*5000*985.2,3*8000*985.2
};

#define MAX_STEPS   256
#define MAX_PATTERN 256
#define PATTERN_LEN 16
#define MAX_SOUNDS  256

struct TRACK
{
    unsigned short PatterNr;
};

struct STEP
{
    TRACK Track[SID_ANZAHL*3];
};

struct PATTERN
{
    unsigned char Note[PATTERN_LEN];        // 0-83 C0 - H6 Wert $ff keine Aktion
    unsigned short SoundNr[PATTERN_LEN];    // SoundNummer
};

struct SOUND
{
    unsigned char Waveform;
    unsigned short Pulsweite;
    unsigned char Attack;
    unsigned char Decay;
    unsigned char Sustain;
    unsigned char Release;
    bool          Ring;
};

class SequenzerClass
{
public:
    SequenzerClass();
    ~SequenzerClass();

    unsigned short OneCycle();
    void SetBPM(int bpm);
    void SetSongLength(int length);
    PATTERN *GetPatternPointer(int nr);
    SOUND   *GetSoundPointer(int nr);
    STEP    *GetStepTablePointer(void);

    void ClearSong(void);
    void Stop(void);
    void Play(void);

private:
    void PushSIDStack(int sid_nr, unsigned char reg_adr, unsigned char reg_wert);
    unsigned short PullSIDStack(void);
    void SetSIDFrequenz(int sid_nr, int voice, unsigned short frequenz);
    void SetSIDPulse(int sid_nr, int voice, unsigned short pulse);
    void NextBeat();
    void PlayTrack(unsigned short pattern_nr ,int sid_nr,int voice);
    void DecrementKeyOffCounters(void);
    void SetDemoSong(void);


    int BPMCounterStart;
    int BPMCounter;
    int TaktCounter;

    int SIDStackLen;
    unsigned short SIDStackPuffer[0x10000];    // Puffergröße 65536 (pro SID 8192 Words)
    unsigned short SIDStackRead;               // Lesezeiger
    unsigned short SIDStackWrite;              // Schreibzeiger

    unsigned short SIDFrequenzen[96];          // Umgerechnete Frequenzwerte für SID Register
    unsigned char ShadowIO[SID_ANZAHL][32];    // SID Registerzwischenspeicher da SID keine READ zulässt

    STEP StepTable[MAX_STEPS];                 // Tabelle mit Patternnummern
    PATTERN Pattern[MAX_PATTERN];              // Tabelle mit Noten und SoundNr
    SOUND Sounds[MAX_SOUNDS];                  // Tabelle mit SoundParametern

    int KeyOffCounter[SID_ANZAHL*3];           // Counter (Wenn 0 wird SID KeyBit auf NULL gesetzt

    bool SongPlay;                             // Wenn TRUE wird StepTabe abespielt
    int SongLaenge;                            // Enthält die Länge(Steps) des Songs
    int StepPos;                               // Aktuelle Position des Sequenzers innerhalb der StepTable
    int PatternPos;                            // Aktuelle Position des Sequenzers innerhalb des akt Pattern
};

#endif // SEQUENZERCLASS_H
