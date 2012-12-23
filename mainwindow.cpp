#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SAMPLERATE 44100
#define PUFFERSIZE 882

#define WaveOutXW (882/2)
#define WaveOutYW (400/2)

void AudioMix(void *userdata, Uint8 *stream, int laenge);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if(ret < 0)
    {
        QMessageBox::critical(0,"SDL Fehler","Fehler beim Installieren von SDL.");
        QApplication::exit(-1);
        return;
    }

    WaveOut = 0;
    WaveOut = SDL_SetVideoMode(WaveOutXW,WaveOutYW,32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(WaveOut == 0)
    {
        QMessageBox::critical(0,"SDL Fehler","Fehler beim setzen des WaveOut Fensters.");
        QApplication::exit(-1);
        return;
    }

    SDL_WM_SetCaption("Wave Output",0);

    SoundBufferL = new double(PUFFERSIZE);
    SoundBufferR = new double(PUFFERSIZE);

    /// SLD Audio Installieren ///
    SDL_AudioSpec format;
    format.freq = SAMPLERATE;
    format.format = AUDIO_S16;
    format.channels = 2;
    format.samples = PUFFERSIZE*2;
    format.userdata = this;
    format.callback = AudioMix;
    format.userdata = this;

    if(SDL_OpenAudio(&format,NULL) < 0)
    {
        QMessageBox::critical(0,"SDL Fehler","Fehler beim Installieren des Sounds.");
        QApplication::exit(-1);
        return;
    }

    /// Sound Starten ///
    SDL_PauseAudio(0);
}

MainWindow::~MainWindow()
{
    SDL_CloseAudio();
    SDL_FreeSurface(WaveOut);

    delete SoundBufferL;
    delete SoundBufferR;

    delete ui;
}

void AudioMix(void *userdata, Uint8 *stream, int laenge)
{
    MainWindow *main = (MainWindow*)userdata;
    main->AudioLoop((short*)stream,laenge);
}

void MainWindow::AudioLoop(short* stream, int laenge)
{
    DrawWaveOut();
}

void MainWindow::DrawWaveOut(void)
{
    // Hintergrund füllen und somit alte Anzeige löschen //
    SDL_FillRect(WaveOut,0,0x00002000);

    // X-Achse
    hlineColor(WaveOut,0,WaveOutXW,WaveOutYW/2,0xe0e000d0);

    double puffer_pos_add = PUFFERSIZE / WaveOutXW;
    double puffer_pos = 0;

    int AktLY = 0;
    int AktRY = 0;
    int OldLY = SoundBufferL[0]*-1 * WaveOutYW/2 + WaveOutYW/2;
    int OldRY = SoundBufferR[0]*-1 * WaveOutYW/2 + WaveOutYW/2;
    for (int i=0;i<WaveOutXW/2;i++)
    {
        AktLY = SoundBufferL[(int)puffer_pos]*-1 * WaveOutYW/2 + WaveOutYW/2;
        aalineColor(WaveOut,i,OldLY,i,AktLY,0x00ff00C0);
        AktRY = SoundBufferR[(int)puffer_pos]*-1 * WaveOutYW/2 + WaveOutYW/2;
        aalineColor(WaveOut,i+WaveOutXW/2,OldRY,i+WaveOutXW/2,AktRY,0x00ff00C0);

        OldLY = AktLY;
        OldRY = AktRY;
        puffer_pos += puffer_pos_add;
    }

    rectangleColor(WaveOut,0,0,WaveOutXW/2,WaveOutYW-1,0xFFFFFF90);
    rectangleColor(WaveOut,WaveOutXW/2+1,0,WaveOutXW-1,WaveOutYW-1,0xFFFFFF90);

    stringColor(WaveOut,2,2,"Links",0xFFFFFFCC);
    stringColor(WaveOut,WaveOutXW/2+3,2,"Rechts",0xFFFFFFCC);

    SDL_Flip(WaveOut);
}
