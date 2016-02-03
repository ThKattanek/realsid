//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: envclass.h                            //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 31.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef ENVCLASS_H
#define ENVCLASS_H

#define	ATTACK		0
#define	DECAY_SUSTAIN   1
#define RELEASE		2

class ENVClass
{
public:
    ENVClass();
    ~ENVClass();
    void Reset(void);
    void OneCycle(void);
    void SetKeyBit(bool value);
    void SetAttackDecay(unsigned char value);
    void SetSustainRelease(unsigned char value);
    unsigned int GetOutput(void);

private:
    bool		KeyBit;
    int			State;
    unsigned int	RatePeriod;
    unsigned int	RateCounter;
    unsigned int	ExponentialCounterPeriod;
    unsigned int	ExponentialCounter;
    unsigned int	EnvCounter;
    bool		HoldZero;
    unsigned int	Attack;
    unsigned int	Decay;
    unsigned int	Sustain;
    unsigned int	Release;
};

#endif // ENVCLASS_H
