//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: mainwindow.h                          //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 27.12.2012		//
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
    void on_Puls0Lo_valueChanged(int value);
    void on_Puls0Hi_valueChanged(int value);
    void on_Tri0_clicked();
    void on_Saw0_clicked();
    void on_Pul0_clicked();
    void on_Nse0_clicked();
    void on_Tst0_clicked();

    void on_Open_SidDump_triggered();
    void on_Play_SidDump_triggered();
    void on_Stop_SidDump_triggered();

private:
    void DrawWaveOut(void);
    Ui::MainWindow *ui;
    SDL_Surface *WaveOut;

    bool DrawingWaveOut;
    bool NoDrawingWaveOut;

    SIDClass *sid;
};

#endif // MAINWINDOW_H
