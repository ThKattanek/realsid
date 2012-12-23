#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

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

private:

    void DrawWaveOut(void);

    Ui::MainWindow *ui;
    SDL_Surface *WaveOut;
    double *SoundBufferL;
    double *SoundBufferR;
};

#endif // MAINWINDOW_H
