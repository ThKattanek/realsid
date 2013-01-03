//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: mainwindow.cpp                        //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 03.01.2013                //
//      					//
//						//
//////////////////////////////////////////////////

#include "filterclass.h"

fc_point f0_points_6581[31]={{0,220},{0,220},{128,230},{256,250},{384,300},{512,420},{640,780},{768,1600},{832,2300},{896,3200},{960,4300},{992,5000},{1008,5400},{1016,5700},{1023,6000},{1023,6000},{1024,4600},{1024,4600},{1032,4800},{1056,5300},{1088,6000},{1120,6600},{1152,7200},{1280,9500},{1408,12000},{1536,14500},{1664,16000},{1792,17100},{1920,17700},{2047,18000},{2047,18000}};
fc_point f0_points_8580[19]={{0,0},{0,0},{128,800},{256,1600},{384,2500},{512,3300},{640,4100},{768,4800},{896,5600},{1024,6500},{1152,7500},{1280,8400},{1408,9200},{1536,9800},{1664,10500},{1792,11000},{1920,11700},{2047,12500},{2047,12500}};

FilterClass::FilterClass()
{
    FilterOn = true;

    interpolate(f0_points_6581, f0_points_6581+ sizeof(f0_points_6581)/sizeof(*f0_points_6581) - 1,PointPlotter<int>(f0_6581), 1.0);
    interpolate(f0_points_8580, f0_points_8580+ sizeof(f0_points_8580)/sizeof(*f0_points_8580) - 1,PointPlotter<int>(f0_8580), 1.0);
}

FilterClass::~FilterClass()
{
}

void FilterClass::Reset()
{
    FilterFrequenz=0;
    FilterResonanz=0;
    FilterKey=0;
    Voice3Off=0;
    HpBpLp=0;
    Volume=0;

    Vhp=0;
    Vbp=0;
    Vlp=0;
    Vnf=0;

    /*
    /// MOS-6581
    WaveZero=0x380;
    VoiceDC=0x800*0xFF;

    MixerDC=-0xfff*0xff/18>>7;
    f0=f0_6581;
    f0_points=f0_points_6581;
    f0_count=sizeof(f0_points_6581)/sizeof(*f0_points_6581);
    */

    /// MOS8580R5
    WaveZero=0x800;
    VoiceDC=0;

    MixerDC=0;
    f0=f0_8580;
    f0_points=f0_points_8580;
    f0_count=sizeof(f0_points_8580)/sizeof(*f0_points_8580);

    SetW0();
    SetQ();
}

void FilterClass::EnableFilter(bool enabled)
{
    FilterOn = enabled;
}

void FilterClass::SetFrequenz(unsigned short frequenz)
{
    FilterFrequenz = frequenz & 0x07FF;
    SetW0();
}

void FilterClass::SetControl1(unsigned char wert)
{
    FilterResonanz=(wert>>4)&0x0F;
    SetQ();
    FilterKey=wert&0x0F;
}

void FilterClass::SetControl2(unsigned char wert)
{
    Voice3Off=wert&0x80;
    HpBpLp=(wert>>4)&0x07;
    Volume=wert&0x0F;
}

void FilterClass::SetW0(void)
{
    const double pi = 3.1415926535897932385;
    w0=(int)(2*pi*f0[FilterFrequenz]*1.048576);
    const int w0_max_1 = static_cast<int>(2*pi*16000*1.048576);
    w0_ceil_1 = w0 <= w0_max_1 ? w0 : w0_max_1;
    const int w0_max_dt = static_cast<int>(2*pi*4000*1.048576);
    w0_ceil_dt = w0 <= w0_max_dt ? w0 : w0_max_dt;
}

void FilterClass::SetQ(void)
{
    _1024_div_Q = static_cast<int>(1024.0/(0.707 + 1.0*FilterResonanz/0x0F));
}

void FilterClass::OneCycle(OSCClass** osc, ENVClass** env)
{
    static int voice1,voice2,voice3,ext_in;

    voice1 = (osc[0]->GetOutput()-WaveZero) * env[0]->GetOutput() + VoiceDC;
    voice2 = (osc[1]->GetOutput()-WaveZero) * env[1]->GetOutput() + VoiceDC;
    voice3 = (osc[2]->GetOutput()-WaveZero) * env[2]->GetOutput() + VoiceDC;

    voice1 >>= 7;
    voice2 >>= 7;

    if(Voice3Off&&!(FilterKey&0x04))
    {
       voice3 = 0;
    }
    else
    {
        voice3 >>= 7;
    }

    ext_in >>= 7;

    if(!FilterOn)
    {
        Vnf = voice1 + voice2 + voice3 + ext_in;
        Vhp = Vbp = Vlp = 0;
        return;
    }

    int Vi;

    switch (FilterKey)
    {
    default:

    case 0x00:
        Vi = 0;
        Vnf = voice1 + voice2 + voice3 + ext_in;
        break;

    case 0x01:
        Vi = voice1;
        Vnf = voice2 + voice3 + ext_in;
        break;

    case 0x02:
        Vi = voice2;
        Vnf = voice1 + voice3 + ext_in;
        break;

    case 0x03:
        Vi = voice1 + voice2;
        Vnf = voice3 + ext_in;
        break;

    case 0x04:
        Vi = voice3;
        Vnf = voice1 + voice2 + ext_in;
        break;

    case 0x05:
        Vi = voice1 + voice3;
        Vnf = voice2 + ext_in;
        break;

    case 0x06:
        Vi = voice2 + voice3;
        Vnf = voice1 + ext_in;
        break;

    case 0x07:
        Vi = voice1 + voice2 + voice3;
        Vnf = ext_in;
        break;

    case 0x08:
        Vi = ext_in;
        Vnf = voice1 + voice2 + voice3;
        break;

    case 0x09:
        Vi = voice1 + ext_in;
        Vnf = voice2 + voice3;
        break;

    case 0x0A:
        Vi = voice2 + ext_in;
        Vnf = voice1 + voice3;
        break;

    case 0x0B:
        Vi = voice1 + voice2 + ext_in;
        Vnf = voice3;
        break;

    case 0x0C:
        Vi = voice3 + ext_in;
        Vnf = voice1 + voice2;
        break;

    case 0x0D:
        Vi = voice1 + voice3 + ext_in;
        Vnf = voice2;
        break;

    case 0x0E:
        Vi = voice2 + voice3 + ext_in;
        Vnf = voice1;
        break;

    case 0x0F:
        Vi = voice1 + voice2 + voice3 + ext_in;
        Vnf = 0;
        break;
    }
    int dVbp = (w0_ceil_1*Vhp>>20);
    int dVlp = (w0_ceil_1*Vbp>>20);
    Vbp-=dVbp;
    Vlp-=dVlp;
    Vhp=(Vbp*_1024_div_Q>>10)-Vlp-Vi;
}

int FilterClass::GetOutput()
{
    if (!FilterOn)
    {
        return (Vnf + MixerDC)*static_cast<int>(Volume);
    }

    int Vf;

    switch(HpBpLp)
    {
    default:

    case 0x00:
        Vf=0;
        break;

    case 0x01:
        Vf=Vlp;
        break;

    case 0x02:
        Vf=Vbp;
        break;

    case 0x03:
        Vf=Vlp+Vbp;
        break;

    case 0x04:
        Vf=Vhp;
        break;

    case 0x05:
        Vf=Vlp+Vhp;
        break;

    case 0x06:
        Vf=Vbp+Vhp;
        break;

    case 0x07:
        Vf=Vlp+Vbp+Vhp;
        break;
    }
    return (Vnf+Vf+MixerDC)*static_cast<int>(Volume);
}
