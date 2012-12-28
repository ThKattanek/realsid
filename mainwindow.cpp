//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: mainwindow.cpp                        //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 27.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SAMPLERATE 48000
#define PUFFERSIZE 882

#define WaveOutXW 400
#define WaveOutYW 200

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
        SoundPuffer[i] = (unsigned short)(SidPuffer[i]* 0.3f * 0xFFFF);
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
    float *sid_puffer = sid->GetSoundPuffer();
    int OldY = sid_puffer[0]*-1 * WaveOutYW/2 + WaveOutYW/2;
    for (int i=0;i<WaveOutXW;i++)
    {
        AktY = sid_puffer[(int)puffer_pos]*-1 * WaveOutYW/2 + WaveOutYW/2;
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

void MainWindow::on_Freq0Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq0Lo_Out->setText(str00);
    sid->WriteIO(0,wert);
}

void MainWindow::on_Freq0Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq0Hi_Out->setText(str00);
    sid->WriteIO(1,wert);
}

void MainWindow::on_Tri0_clicked()
{
    unsigned char wave = 0;
    if(ui->Tri0->isChecked()) wave |= 16;
    if(ui->Saw0->isChecked()) wave |= 32;
    if(ui->Pul0->isChecked()) wave |= 64;
    if(ui->Nse0->isChecked()) wave |= 128;
    sid->WriteIO(4,wave);
}

void MainWindow::on_Saw0_clicked()
{
    unsigned char wave = 0;
    if(ui->Tri0->isChecked()) wave |= 16;
    if(ui->Saw0->isChecked()) wave |= 32;
    if(ui->Pul0->isChecked()) wave |= 64;
    if(ui->Nse0->isChecked()) wave |= 128;
    sid->WriteIO(4,wave);
}

void MainWindow::on_Pul0_clicked()
{
    unsigned char wave = 0;
    if(ui->Tri0->isChecked()) wave |= 16;
    if(ui->Saw0->isChecked()) wave |= 32;
    if(ui->Pul0->isChecked()) wave |= 64;
    if(ui->Nse0->isChecked()) wave |= 128;
    sid->WriteIO(4,wave);
}

void MainWindow::on_Nse0_clicked()
{
    unsigned char wave = 0;
    if(ui->Tri0->isChecked()) wave |= 16;
    if(ui->Saw0->isChecked()) wave |= 32;
    if(ui->Pul0->isChecked()) wave |= 64;
    if(ui->Nse0->isChecked()) wave |= 128;
    sid->WriteIO(4,wave);
}

void MainWindow::on_Open_SidDump_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Emu64 SID Dump öffnen "),"",tr("Emu64 SID Dump Datei") + "(*.sdp);;" + tr("Alle Dateien") + "(*.*)");
    if(filename != "")
    {
        if(!sid->OpenSIDDump(filename.toAscii().data()))
            QMessageBox::warning(this,"realSID Error !","Fehler beim öffnen des SID Dump Files.");
    }
}

void MainWindow::on_Play_SidDump_triggered()
{
    sid->PlaySIDDump();
}

void MainWindow::on_Stop_SidDump_triggered()
{
    sid->StopSIDDump();
}

void MainWindow::on_Check_MultiWave_clicked()
{
    if(sid->CheckMultiWave()) QMessageBox::information(this,"realSID Info","Es sind Mischwellen vorgekommen.");
    else QMessageBox::information(this,"realSID Info","Es sind keine Mischwellen vorgekommen.");
}

void MainWindow::on_Reset_MultiWave_clicked()
{
    sid->ResetMultiWave();
}

void MainWindow::on_Puls0Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls0Lo_Out->setText(str00);
    sid->WriteIO(2,wert);
}

void MainWindow::on_Puls0Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls0Hi_Out->setText(str00);
    sid->WriteIO(3,wert);
}
