//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: mainwindow.cpp                        //
//						//
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek		//
//						//
// Letzte Änderung am 23.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SAMPLERATE 44100
#define PUFFERSIZE 882*2

#define WaveOutXW 200
#define WaveOutYW 100

void AudioMix(void *userdata, Uint8 *stream, int laenge);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    WaveOut(NULL),
    NoDrawingWaveOut(true)
{
    ui->setupUi(this);


    int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if(ret < 0)
    {
        QMessageBox::critical(0,"SDL Fehler","Fehler beim Installieren von SDL.");
        QApplication::exit(-1);
        return;
    }

    sid = new SIDClass(SAMPLERATE,PUFFERSIZE);

    /// SLD Audio Installieren ///
    SDL_AudioSpec format;
    format.freq = SAMPLERATE;
    format.format = AUDIO_S16;
    format.channels = 1;
    format.samples = PUFFERSIZE;
    format.userdata = this;
    format.callback = AudioMix;

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
    ShowWaveOut(false);
    SDL_CloseAudio();

    delete sid;
    delete ui;
}

void AudioMix(void *userdata, Uint8 *stream, int laenge)
{
    MainWindow *main = (MainWindow*)userdata;
    main->AudioLoop((short*)stream,laenge);
}

void MainWindow::AudioLoop(short* stream, int laenge)
{
    sid->ResetSoundPufferPos();

    while(sid->GetSoundPufferPos() < (laenge/2))
    {
        sid->OneCycle();
    }

    unsigned short *SoundPuffer = (unsigned short*)stream;
    float *SidPuffer = sid->GetSoundPuffer();

    for(int i=0; i<(laenge/2);i++)
    {
        SoundPuffer[i] = (unsigned short)(SidPuffer[i] * 10256);
    }
    DrawWaveOut();
}

void MainWindow::ShowWaveOut(bool enabled)
{
    if((enabled == true) && (WaveOut == NULL))
    {
        WaveOut = SDL_SetVideoMode(WaveOutXW,WaveOutYW,32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        if(WaveOut == 0)
        {
            QMessageBox::critical(0,"SDL Fehler","Fehler beim setzen des WaveOut Fensters.");
            QApplication::exit(-1);
            return;
        }
        else
        {
            SDL_WM_SetCaption("Wave Output [SDL]",0);
            NoDrawingWaveOut = false;
        }

    }
    else
    {
        if(WaveOut != NULL)
        {
            DrawingWaveOut = true;
            NoDrawingWaveOut = true;

            while(DrawingWaveOut)
            {
                SDL_Delay(1);
            };

            SDL_FreeSurface(WaveOut);
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
            WaveOut = NULL;
        }
    }
}

void MainWindow::DrawWaveOut(void)
{
    if(NoDrawingWaveOut)
    {
        DrawingWaveOut = false;
        return;
    }

    // Hintergrund füllen und somit alte Anzeige löschen //
    SDL_FillRect(WaveOut,0,0x00002000);

    // X-Achse
    hlineColor(WaveOut,0,WaveOutXW,WaveOutYW/2,0xe0e000d0);

    float puffer_pos_add = PUFFERSIZE/2 / (WaveOutXW);
    float puffer_pos = 0;

    int AktY = 0;
    int OldY = sid->SoundPuffer[0]*-1 * WaveOutYW/2 + WaveOutYW/2;
    for (int i=0;i<WaveOutXW;i++)
    {
        AktY = sid->SoundPuffer[(int)puffer_pos]*-1 * WaveOutYW/2 + WaveOutYW/2;
        aalineColor(WaveOut,i,OldY,i,AktY,0x00ff00C0);

        OldY = AktY;
        puffer_pos += puffer_pos_add;
    }

    rectangleColor(WaveOut,0,0,WaveOutXW,WaveOutYW-1,0xFFFFFF90);
    SDL_Flip(WaveOut);
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    ShowWaveOut(checked);
}
