//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: mainwindow.h                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 31.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include <sidclass.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void AudioLoop(short int *stream, int laenge);
    void ShowWaveOut(bool enabled);

private slots:
    void on_checkBox_clicked(bool checked);

    void on_Freq0Lo_valueChanged(int value);
    void on_Freq0Hi_valueChanged(int value);
    void on_Freq1Lo_valueChanged(int value);
    void on_Freq1Hi_valueChanged(int value);
    void on_Freq2Lo_valueChanged(int value);
    void on_Freq2Hi_valueChanged(int value);
    void on_Puls0Lo_valueChanged(int value);
    void on_Puls0Hi_valueChanged(int value);
    void on_Puls1Lo_valueChanged(int value);
    void on_Puls1Hi_valueChanged(int value);
    void on_Puls2Lo_valueChanged(int value);
    void on_Puls2Hi_valueChanged(int value);

    void SetCtrlReg(int voice);

    void on_Tri0_clicked();
    void on_Saw0_clicked();
    void on_Pul0_clicked();
    void on_Nse0_clicked();
    void on_Tst0_clicked();
    void on_Sync0_clicked();
    void on_Ring0_clicked();
    void on_Key0_clicked();

    void on_Tri1_clicked();
    void on_Saw1_clicked();
    void on_Pul1_clicked();
    void on_Nse1_clicked();
    void on_Tst1_clicked();
    void on_Sync1_clicked();
    void on_Ring1_clicked();
    void on_Key1_clicked();

    void on_Tri2_clicked();
    void on_Saw2_clicked();
    void on_Pul2_clicked();
    void on_Nse2_clicked();
    void on_Tst2_clicked();
    void on_Sync2_clicked();
    void on_Ring2_clicked();
    void on_Key2_clicked();

    void on_Open_SidDump_triggered();
    void on_Play_SidDump_triggered();
    void on_Stop_SidDump_triggered();

    void on_Attack0_valueChanged(int value);
    void on_Decay0_valueChanged(int value);
    void on_Sustain0_valueChanged(int value);
    void on_Release0_valueChanged(int value);

    void on_Attack1_valueChanged(int value);
    void on_Decay1_valueChanged(int value);
    void on_Sustain1_valueChanged(int value);
    void on_Release1_valueChanged(int value);

    void on_Attack2_valueChanged(int value);
    void on_Decay2_valueChanged(int value);
    void on_Sustain2_valueChanged(int value);
    void on_Release2_valueChanged(int value);

private:
    void DrawWaveOut(void);
    Ui::MainWindow *ui;
    SDL_Surface *WaveOut;

    bool DrawingWaveOut;
    bool NoDrawingWaveOut;

    SIDClass *sid;
};

#endif // MAINWINDOW_H
