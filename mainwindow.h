//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: mainwindow.h                          //
//						//
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek		//
//						//
// Letzte Änderung am 23.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

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

private:
    void DrawWaveOut(void);
    Ui::MainWindow *ui;
    SDL_Surface *WaveOut;

    bool DrawingWaveOut;
    bool NoDrawingWaveOut;

    SIDClass *sid;
};

#endif // MAINWINDOW_H
