//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sequenzerwindow.h                     //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 12.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef SEQUENZERWINDOW_H
#define SEQUENZERWINDOW_H

#include <QMainWindow>
#include <sequenzerclass.h>
#include <QList>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QVariant>

#include <notenedit.h>
#include <spinbox.h>

namespace Ui {
    class SequenzerWindow;
}

class SequenzerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SequenzerWindow(QWidget *parent = 0, SequenzerClass *_seq = 0);
    ~SequenzerWindow();

private slots:
    void onBPMChange(int value);
    void onPlaySongClick();
    void onStopSongClick();
    void onClearSongClick();
    void onSoundNrChange(int id, int value);
    void onNoteChange(int id, int value);
    void onEffektNrChange(int id, int value);
    void onEffektPara1Change(int id, int value);
    void onV1StepPatternChange(int id, int value);
    void onV2StepPatternChange(int id, int value);
    void onV3StepPatternChange(int id, int value);
    void onV1StepTransposeChange(int id, int value);
    void onV2StepTransposeChange(int id, int value);
    void onV3StepTransposeChange(int id, int value);
    void onSongLengthChange(int value);
    void onLoadSongClick();
    void onSaveSongClick();
    void onTimer1();
    void on_PatternNr_valueChanged(int value);
    void on_SoundNr_valueChanged(int value);
    void on_Attack_valueChanged(int value);
    void on_Decay_valueChanged(int value);
    void on_Sustain_valueChanged(int value);
    void on_Release_valueChanged(int value);
    void on_WaveForm_currentIndexChanged(int index);
    void on_Pulsweite_valueChanged(int value);

    void on_ClearPattern_clicked();

    void on_ClearSound_clicked();

private:
    void FillSongParameter(void);
    void FillStepList(void);
    void FillPatternList(void);
    void FillSoundFrame(void);

    Ui::SequenzerWindow *ui;
    SequenzerClass *seq;
    QSpinBox *BPM;
    QSpinBox *SONGSIZE;
    QPushButton *PlaySong;
    QPushButton *StopSong;
    QPushButton *ClearSong;
    QPushButton *LoadSong;
    QPushButton *SaveSong;
    QStringList list0;
    QStringList list1;

    STEP    *StepTable;
    PATTERN *AktPattern;
    SOUND   *AktSound;

    QTimer *timer1;
};

#endif // SEQUENZERWINDOW_H
