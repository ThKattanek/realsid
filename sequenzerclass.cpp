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

    for(int i=0;i<24;i++)
        KeyOffCounter[i] = 0;

    ClearSong();
    SetDemoSong();
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

        for(int sid_nr=0;sid_nr<8;sid_nr++)
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
    if(bpm == 0) BPMCounterStart = 0;
    else BPMCounterStart = (PAL_TAKT * 60) / bpm;
    BPMCounter = BPMCounterStart;
}

PATTERN* SequenzerClass::GetPatternPointer(int nr)
{
    if(nr >= MAX_PATTERN) return 0;
    return &Pattern[nr];
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

void SequenzerClass::ClearSong()
{
    SongPlay = false;

    /// Songlänge = 0
    SongLaenge = 0;
    StepPos = 0;
    PatternPos = 0;

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
        for(int j=0;j<24;j++)
            StepTable[i].Track[j].PatterNr = 0xffff;
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
    SongLaenge = 1;
    StepTable[0].Track[0].PatterNr = 0;
    StepTable[0].Track[1].PatterNr = 1;

    Sounds[0].Attack = 0x00;
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

    /*
    Pattern[0].Note[0]=33;
    Pattern[0].KeyOff[1] = true;
    Pattern[0].Note[8]=38;
    Pattern[0].KeyOff[9] = true;
    Pattern[0].Note[16]=33;
    Pattern[0].KeyOff[17] = true;
    Pattern[0].Note[24]=38;
    Pattern[0].KeyOff[25] = true;
    */
}

void SequenzerClass::NextBeat()
{
    if(SongLaenge == 0) return;

    for(int sid_nr=0;sid_nr<8;sid_nr++)
    {
        for(int voice=0;voice<3;voice++)
            PlayTrack(StepTable[StepPos].Track[sid_nr*3+voice].PatterNr,sid_nr,voice);
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

void SequenzerClass::PlayTrack(unsigned short pattern_nr, int sid_nr, int voice)
{
    if(pattern_nr == 0xffff) return;

    unsigned char Note = Pattern[pattern_nr].Note[PatternPos];
    unsigned short SoundNr = Pattern[pattern_nr].SoundNr[PatternPos];

    PushSIDStack(sid_nr,24,15);

    if(Note == 0xff) return;

    SetSIDFrequenz(sid_nr,voice,SIDFrequenzen[Note]);
    PushSIDStack(sid_nr,voice*7+5,(Sounds[SoundNr].Attack<<4) | (Sounds[SoundNr].Decay));
    PushSIDStack(sid_nr,voice*7+6,(Sounds[SoundNr].Sustain<<4) | (Sounds[SoundNr].Release));
    PushSIDStack(sid_nr,voice*7+4,(ShadowIO[sid_nr][voice*7+4] & 0x0D) | (Sounds[SoundNr].Waveform<<4) | 1);

    KeyOffCounter[sid_nr*3+voice] = AttackTime[Sounds[SoundNr].Attack]+DecayTime[Sounds[SoundNr].Decay];
}

void SequenzerClass::DecrementKeyOffCounters()
{
    for(int sid_nr=0;sid_nr<8;sid_nr++)
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
