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

#ifndef FILTERCLASS_H
#define FILTERCLASS_H

#include <oscclass.h>
#include <envclass.h>
#include <filtercalc.h>

typedef int fc_point[2];

class FilterClass
{
public:
    FilterClass();
    ~FilterClass();
    void Reset(void);
    void OneCycle(OSCClass** osc, ENVClass** env);
    void EnableFilter(bool enabled);
    int  GetOutput(void);
    void SetFrequenz(unsigned short filter_frequenz);
    void SetControl1(unsigned char wert);
    void SetControl2(unsigned char wert);

private:
    void SetW0(void);
    void SetQ(void);

    bool                FilterOn;

    unsigned int	VoiceDC;
    unsigned int	WaveZero;

    unsigned int	FilterKey;
    unsigned int	FilterFrequenz;
    unsigned int	FilterResonanz;
    unsigned int	Voice3Off;
    unsigned int	HpBpLp;
    unsigned int	Volume;

    int                 MixerDC;
    int                 Vhp;
    int                 Vbp;
    int                 Vlp;
    int                 Vnf;
    int                 w0,w0_ceil_1,w0_ceil_dt;
    int                 _1024_div_Q;
    int                 f0_6581[2048];
    int                 f0_8580[2048];
    int*		f0;
    fc_point*           f0_points;
    int			f0_count;
};

#endif // FILTERCLASS_H
