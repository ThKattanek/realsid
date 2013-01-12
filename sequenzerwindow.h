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
    void onV1StepChange(int id, int value);
    void onV2StepChange(int id, int value);
    void onV3StepChange(int id, int value);
    void onSongLengthChange(int value);
    void on_PatternNr_valueChanged(int value);
    void on_SoundNr_valueChanged(int value);
    void on_Attack_valueChanged(int value);
    void on_Decay_valueChanged(int value);
    void on_Sustain_valueChanged(int value);
    void on_Release_valueChanged(int value);
    void on_WaveForm_currentIndexChanged(int index);
    void on_Pulsweite_valueChanged(int value);

private:
    void FillPatternList(void);
    void FillSoundFrame(void);

    Ui::SequenzerWindow *ui;
    SequenzerClass *seq;
    QSpinBox *BPM;
    QSpinBox *SONGSIZE;
    QPushButton *PlaySong;
    QPushButton *StopSong;
    QPushButton *ClearSong;
    QStringList list;

    STEP    *StepTable;
    PATTERN *AktPattern;
    SOUND   *AktSound;
};

#endif // SEQUENZERWINDOW_H
