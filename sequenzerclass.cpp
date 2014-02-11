//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sequenzerclass.cpp                    //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 12.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#include "sequenzerclass.h"
#include <fstream>
#include <cstring>

SequenzerClass::SequenzerClass()
{
    TaktCounter = 0;
    SIDStackLen = 0;

    SIDStackRead = 0;
    SIDStackWrite = 0;

    BPMCounterStart = (985248 * 60) / 120;   // Auf 120 BPM setzen
    BPMCounter = BPMCounterStart;

    for(int i=0;i<96;i++)
    {
        SIDFrequenzen[i] = (NotenFrequenzen[i] * 16777216) /  PAL_TAKT;
    }

    for(int i=0;i<(SID_ANZAHL*3);i++)
        KeyOffCounter[i] = 0;

    ClearSong();
}

SequenzerClass::~SequenzerClass()
{
}

unsigned short SequenzerClass::OneCycle()
{
    static bool SongPlayOld;

    DecrementKeyOffCounters();

    if((SongPlayOld==true) && (SongPlay==false))
    {
        /// SongPlay geht von 1 auf 0 ///
        /// Alle SID Register löschen !!

        for(int sid_nr=0;sid_nr<SID_ANZAHL;sid_nr++)
        {
            for(int i=0;i<29;i++) PushSIDStack(sid_nr,i,0);
        }
        StepPos = 0;
        PatternPos =0;
    }

    SongPlayOld = SongPlay;

    if(SongPlay)
    {
        BPMCounter--;
        if(BPMCounter == 0)
        {
            BPMCounter = BPMCounterStart;

            /// Takt BPM ab hier ///
            NextBeat();
        }

        /*
        if(++TaktCounter == 19705)
        {
            TaktCounter = 0;

            //// Ab hier 50 mal in der Sekunde
        }
        */
    }
    return PullSIDStack();
}

void SequenzerClass::SetBPM(int bpm)
{
    BPM = bpm;
    if(bpm == 0) BPMCounterStart = 0;
    else BPMCounterStart = (PAL_TAKT * 60) / bpm;
    BPMCounter = BPMCounterStart;
}

int SequenzerClass::GetBPM()
{
    return BPM;
}

void SequenzerClass::SetSongLength(int length)
{
    if(length >= MAX_STEPS) return;
    SongPlay = false;
    StepPos = 0;
    PatternPos = 0;
    SongLaenge = length;
}

int SequenzerClass::GetSongLength()
{
    return SongLaenge;
}

int SequenzerClass::GetAktStepPos()
{
    return StepPos;
}

int SequenzerClass::GetAktPatternPos()
{
    return PatternPos;
}

PATTERN* SequenzerClass::GetPatternPointer(int nr)
{
    if(nr >= MAX_PATTERN) return 0;
    return &Pattern[nr];
}

SOUND* SequenzerClass::GetSoundPointer(int nr)
{
    if(nr >= MAX_SOUNDS) return 0;
    return &Sounds[nr];
}

STEP* SequenzerClass::GetStepTablePointer()
{
    return StepTable;
}

void SequenzerClass::Play()
{
    SongPlay = true;
}

void SequenzerClass::Stop()
{
    SongPlay = false;
}

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Private Funktionen ////////////////////////////////////////

void SequenzerClass::PushSIDStack(int sid_nr, unsigned char reg_adr, unsigned char reg_wert)
{
    if(SIDStackLen == 0xFFFF) return;

    ShadowIO[sid_nr][reg_adr] = reg_wert;

    SIDStackLen++;
    SIDStackPuffer[SIDStackWrite] = ((sid_nr & 0x07) << 13) | ((reg_adr & 0x1f) << 8) | reg_wert;
    SIDStackWrite++;
}

unsigned short SequenzerClass::PullSIDStack()
{
    if(SIDStackLen == 0) return 0x1f;

    SIDStackLen--;
    unsigned short ret = SIDStackPuffer[SIDStackRead];
    SIDStackRead++;
    return ret;
}

void SequenzerClass::SetSIDFrequenz(int sid_nr, int voice, unsigned short frequenz)
{
    PushSIDStack(sid_nr,voice*7+0,frequenz & 0xff);
    PushSIDStack(sid_nr,voice*7+1,frequenz >> 8);
}

void SequenzerClass::SetSIDPulse(int sid_nr, int voice, unsigned short pulse)
{
    PushSIDStack(sid_nr,voice*7+2,pulse & 0xff);
    PushSIDStack(sid_nr,voice*7+3,pulse >> 8);
}

void SequenzerClass::ClearSong()
{
    SongPlay = false;

    /// Songlänge = 0
    SongLaenge = 0;
    StepPos = 0;
    PatternPos = 0;

    SetBPM(120);

    /// Alle Souns löschen
    for(int i=0;i<MAX_SOUNDS;i++)
    {
        Sounds[i].Waveform = 0;
        Sounds[i].Pulsweite = 0;
        Sounds[i].Attack = 0;
        Sounds[i].Decay = 0;
        Sounds[i].Sustain = 0;
        Sounds[i].Release = 0;
    }

    /// StepTable löschen
    for(int i=0;i<MAX_STEPS;i++)
    {
        for(int j=0;j<(SID_ANZAHL*3);j++)
        {
            StepTable[i].Track[j].PatterNr = 0;
            StepTable[i].Track[j].Transpose = 0;
        }
    }

    /// Pattern löschen
    for(int i=0;i<MAX_PATTERN;i++)
    {
        for(int j=0;j<PATTERN_LEN;j++)
        {
            Pattern[i].Note[j] = 0xff;
            Pattern[i].SoundNr[j] = 0;
        }
    }
}

void SequenzerClass::SetDemoSong()
{
    SongLaenge = 2;
    SetBPM(120);

    StepTable[0].Track[0].Transpose = -25;
    StepTable[0].Track[0].PatterNr = 0;
    StepTable[0].Track[1].PatterNr = 1;
    StepTable[1].Track[0].Transpose = -20;
    StepTable[1].Track[0].PatterNr = 0;
    StepTable[1].Track[1].PatterNr = 1;

    Sounds[0].Attack = 0x04;
    Sounds[0].Decay = 0x04;
    Sounds[0].Sustain = 0x0f;
    Sounds[0].Release = 0x06;
    Sounds[0].Waveform = 2;
    Sounds[0].Pulsweite = 0x7fff;

    Sounds[1].Attack = 0x00;
    Sounds[1].Decay = 0x04;
    Sounds[1].Sustain = 0x0f;
    Sounds[1].Release = 0x08;
    Sounds[1].Waveform = 8;
    Sounds[1].Pulsweite = 0x7fff;

    for(int i=0;i<PATTERN_LEN/2;i++)
    {
        Pattern[0].Note[i*2+0]=43;
        Pattern[0].SoundNr[i*2+0]=0;
        Pattern[0].Note[i*2+1]=55;
        Pattern[0].SoundNr[i*2+1]=0;
    }

    for(int i=0;i<PATTERN_LEN/2;i++)
    {
        Pattern[1].Note[i*2+0]=25;
        Pattern[1].SoundNr[i*2+0]=1;
    }
}

void SequenzerClass::NextBeat()
{
    if(SongLaenge == 0) return;

    bool break_found = false;

    /// Nach XXX suchen
    for(int sid_nr=0;sid_nr<SID_ANZAHL;sid_nr++)
    {
        for(int voice=0;voice<3;voice++)
        {
            int PatternNr = StepTable[StepPos].Track[sid_nr*3+voice].PatterNr;
            if(Pattern[PatternNr].Note[PatternPos] == 254) break_found = true;
        }
    }

    if(break_found)
    {
        PatternPos = 0;
        StepPos++;
        if(StepPos == SongLaenge)
            StepPos = 0;
    }

    for(int sid_nr=0;sid_nr<SID_ANZAHL;sid_nr++)
    {
        for(int voice=0;voice<3;voice++)
        {
            PlayTrack(StepTable[StepPos].Track[sid_nr*3+voice].PatterNr,StepTable[StepPos].Track[sid_nr*3+voice].Transpose,sid_nr,voice);
        }
    }

    PatternPos++;
    if(PatternPos == PATTERN_LEN)
    {
        PatternPos = 0;
        StepPos++;
        if(StepPos == SongLaenge)
            StepPos = 0;
    }

}

void SequenzerClass::PlayTrack(unsigned short pattern_nr,char transpose, int sid_nr, int voice)
{
    if(pattern_nr == 0xffff) return;

    unsigned char Note = Pattern[pattern_nr].Note[PatternPos];
    unsigned short SoundNr = Pattern[pattern_nr].SoundNr[PatternPos];

    unsigned char EffektNr = Pattern[pattern_nr].EffektNr[PatternPos];
    unsigned short EffektPara1 = Pattern[pattern_nr].EffektParameter1[PatternPos];

    switch(EffektNr)
    {
    case 0:
        /// Nichts tun
        break;
    case 1:
        /// SetVolume
        PushSIDStack(sid_nr,24,(ShadowIO[sid_nr][24] & 0xf0) | (EffektPara1 & 0x0f));
        break;
    case 2:
        /// SetFilterFrequenz
        EffektPara1 &= 0x7ff;
        PushSIDStack(sid_nr,21,EffektPara1 & 7);
        PushSIDStack(sid_nr,22,EffektPara1 >> 3);
        break;
    case 3:
        /// FilterResonanz
        EffektPara1 &= 0x0f;
        PushSIDStack(sid_nr,23,(ShadowIO[sid_nr][23] & 0x0f) | (EffektPara1 & 0x0f)<<4);
        break;
    case 4:
        /// FilterOn
        EffektPara1 &= 0x03;

        switch(EffektPara1)
        {
        case 0:
            PushSIDStack(sid_nr,23,ShadowIO[sid_nr][23] | 1);
            break;
        case 1:
            PushSIDStack(sid_nr,23,ShadowIO[sid_nr][23] | 2);
            break;
        case 2:
            PushSIDStack(sid_nr,23,ShadowIO[sid_nr][23] | 4);
            break;
        }
        break;
    case 5:
        /// FilterOff
        EffektPara1 &= 0x03;

        switch(EffektPara1)
        {
        case 0:
            PushSIDStack(sid_nr,23,ShadowIO[sid_nr][23] & 254);
            break;
        case 1:
            PushSIDStack(sid_nr,23,ShadowIO[sid_nr][23] & 253);
            break;
        case 2:
            PushSIDStack(sid_nr,23,ShadowIO[sid_nr][23] & 251);
            break;
        }
        break;
    case 6:
        /// Filtertyp
        EffektPara1 &= 0x0f;
        PushSIDStack(sid_nr,24,(ShadowIO[sid_nr][24] & 0x0f) | ((1<<EffektPara1)<<3)&0xf0);
        break;
    }

    if(Note == 0xff) return;

    SetSIDFrequenz(sid_nr,voice,SIDFrequenzen[Note+transpose]);
    SetSIDPulse(sid_nr,voice,Sounds[SoundNr].Pulsweite);
    PushSIDStack(sid_nr,voice*7+5,(Sounds[SoundNr].Attack<<4) | (Sounds[SoundNr].Decay));
    PushSIDStack(sid_nr,voice*7+6,(Sounds[SoundNr].Sustain<<4) | (Sounds[SoundNr].Release));
    PushSIDStack(sid_nr,voice*7+4,(ShadowIO[sid_nr][voice*7+4] & 0x0D) | (Sounds[SoundNr].Waveform<<4) | 1);

    KeyOffCounter[sid_nr*3+voice] = AttackTime[Sounds[SoundNr].Attack]+DecayTime[Sounds[SoundNr].Decay];
}

void SequenzerClass::DecrementKeyOffCounters()
{
    for(int sid_nr=0;sid_nr<SID_ANZAHL;sid_nr++)
    {
        for(int voice=0;voice<3;voice++)
        {
            if(KeyOffCounter[sid_nr*3+voice]>0)
            {
                KeyOffCounter[sid_nr*3+voice]--;
                if(KeyOffCounter[sid_nr*3+voice] == 0)
                {
                    /// Note Abschalten ///
                    PushSIDStack(sid_nr,voice*7+4,ShadowIO[sid_nr][voice*7+4] & 0xFE);
                }
            }
        }
    }
}

bool SequenzerClass::LoadSong(char *filename)
{
    FILE* file = NULL;

    file = fopen (filename,"rb");
    if (file == NULL)
    {
        return false;
    }

    char Kennung[10];
    fread (&Kennung,1,10,file);

    if(0 != strcmp(Kennung,"reSID_SEQ"))
    {
        fclose(file);
        return false;
    }

    Stop();

    unsigned short Version;
    fread(&Version,sizeof(Version),1,file);
    if(Version == 0x0001)
    {
        //// Version 0001

        fread (&SongLaenge,sizeof(SongLaenge),1,file);
        fread (&BPM,sizeof(BPM),1,file);
        SetBPM(BPM);

        fread (StepTable,sizeof(StepTable),1,file);
        fread (Pattern,sizeof(Pattern),1,file);
        fread (Sounds,sizeof(Sounds),1,file);
    }

    fclose(file);
    return true;
}

bool SequenzerClass::SaveSong(char *filename)
{
    FILE* file = NULL;

    file = fopen (filename,"wb");
    if (file == NULL)
    {
        return false;
    }

    char Kennung[]={"reSID_SEQ"};
    fwrite (&Kennung,1,10,file);

    unsigned short Version = 0x0001;
    fwrite(&Version,sizeof(Version),1,file);


    fwrite (&SongLaenge,sizeof(SongLaenge),1,file);
    fwrite (&BPM,sizeof(BPM),1,file);

    fwrite (StepTable,sizeof(StepTable),1,file);
    fwrite (Pattern,sizeof(Pattern),1,file);
    fwrite (Sounds,sizeof(Sounds),1,file);

    fclose(file);
    return true;
}
