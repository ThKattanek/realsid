//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: mainwindow.cpp                        //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 31.12.2012		//
//      					//
//						//
//////////////////////////////////////////////////

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SAMPLERATE 48000
#define PUFFERSIZE 1920

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
        SoundPuffer[i] = (unsigned short)(SidPuffer[i]* 0.5f * 0xFFFF);
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

void MainWindow::on_Freq1Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq1Lo_Out->setText(str00);
    sid->WriteIO(7,wert);
}

void MainWindow::on_Freq1Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq1Hi_Out->setText(str00);
    sid->WriteIO(8,wert);
}

void MainWindow::on_Freq2Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq2Lo_Out->setText(str00);
    sid->WriteIO(14,wert);
}

void MainWindow::on_Freq2Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq2Hi_Out->setText(str00);
    sid->WriteIO(15,wert);
}

void MainWindow::SetCtrlReg(int voice)
{
    unsigned char ctrl = 0;

    switch(voice)
    {
    case 0:
        if(ui->Key0->isChecked()) ctrl |= 1;
        if(ui->Sync0->isChecked()) ctrl |= 2;
        if(ui->Ring0->isChecked()) ctrl |= 4;
        if(ui->Tst0->isChecked()) ctrl |= 8;
        if(ui->Tri0->isChecked()) ctrl |= 16;
        if(ui->Saw0->isChecked()) ctrl |= 32;
        if(ui->Pul0->isChecked()) ctrl |= 64;
        if(ui->Nse0->isChecked()) ctrl |= 128;
        sid->WriteIO(4,ctrl);
        break;
    case 1:
        if(ui->Key1->isChecked()) ctrl |= 1;
        if(ui->Sync1->isChecked()) ctrl |= 2;
        if(ui->Ring1->isChecked()) ctrl |= 4;
        if(ui->Tst1->isChecked()) ctrl |= 8;
        if(ui->Tri1->isChecked()) ctrl |= 16;
        if(ui->Saw1->isChecked()) ctrl |= 32;
        if(ui->Pul1->isChecked()) ctrl |= 64;
        if(ui->Nse1->isChecked()) ctrl |= 128;
        sid->WriteIO(11,ctrl);
        break;
    case 2:
        if(ui->Key2->isChecked()) ctrl |= 1;
        if(ui->Sync2->isChecked()) ctrl |= 2;
        if(ui->Ring2->isChecked()) ctrl |= 4;
        if(ui->Tst2->isChecked()) ctrl |= 8;
        if(ui->Tri2->isChecked()) ctrl |= 16;
        if(ui->Saw2->isChecked()) ctrl |= 32;
        if(ui->Pul2->isChecked()) ctrl |= 64;
        if(ui->Nse2->isChecked()) ctrl |= 128;
        sid->WriteIO(18,ctrl);
        break;
    default:
        break;
    }
}

void MainWindow::on_Tri0_clicked(){SetCtrlReg(0);}
void MainWindow::on_Saw0_clicked(){SetCtrlReg(0);}
void MainWindow::on_Pul0_clicked(){SetCtrlReg(0);}
void MainWindow::on_Nse0_clicked(){SetCtrlReg(0);}
void MainWindow::on_Tst0_clicked(){SetCtrlReg(0);}
void MainWindow::on_Sync0_clicked(){SetCtrlReg(0);}
void MainWindow::on_Ring0_clicked(){SetCtrlReg(0);}
void MainWindow::on_Key0_clicked(){SetCtrlReg(0);}

void MainWindow::on_Tri1_clicked(){SetCtrlReg(1);}
void MainWindow::on_Saw1_clicked(){SetCtrlReg(1);}
void MainWindow::on_Pul1_clicked(){SetCtrlReg(1);}
void MainWindow::on_Nse1_clicked(){SetCtrlReg(1);}
void MainWindow::on_Tst1_clicked(){SetCtrlReg(1);}
void MainWindow::on_Sync1_clicked(){SetCtrlReg(1);}
void MainWindow::on_Ring1_clicked(){SetCtrlReg(1);}
void MainWindow::on_Key1_clicked(){SetCtrlReg(1);}

void MainWindow::on_Tri2_clicked(){SetCtrlReg(2);}
void MainWindow::on_Saw2_clicked(){SetCtrlReg(2);}
void MainWindow::on_Pul2_clicked(){SetCtrlReg(2);}
void MainWindow::on_Nse2_clicked(){SetCtrlReg(2);}
void MainWindow::on_Tst2_clicked(){SetCtrlReg(2);}
void MainWindow::on_Sync2_clicked(){SetCtrlReg(2);}
void MainWindow::on_Ring2_clicked(){SetCtrlReg(2);}
void MainWindow::on_Key2_clicked(){SetCtrlReg(2);}

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

void MainWindow::on_Puls1Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls1Lo_Out->setText(str00);
    sid->WriteIO(9,wert);
}

void MainWindow::on_Puls1Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls1Hi_Out->setText(str00);
    sid->WriteIO(10,wert);
}

void MainWindow::on_Puls2Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls2Lo_Out->setText(str00);
    sid->WriteIO(16,wert);
}

void MainWindow::on_Puls2Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls2Hi_Out->setText(str00);
    sid->WriteIO(17,wert);
}

void MainWindow::on_Attack0_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack0->value() & 0xf) << 4) | (ui->Decay0->value() & 0xf);
    sid->WriteIO(5,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Attack0_Out->setText(str00);
}

void MainWindow::on_Decay0_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack0->value() & 0xf) << 4) | (ui->Decay0->value() & 0xf);
    sid->WriteIO(5,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Decay0_Out->setText(str00);
}

void MainWindow::on_Sustain0_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain0->value() & 0xf) << 4) | (ui->Release0->value() & 0xf);
    sid->WriteIO(6,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Sustain0_Out->setText(str00);
}

void MainWindow::on_Release0_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain0->value() & 0xf) << 4) | (ui->Release0->value() & 0xf);
    sid->WriteIO(6,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Release0_Out->setText(str00);
}

void MainWindow::on_Attack1_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack1->value() & 0xf) << 4) | (ui->Decay1->value() & 0xf);
    sid->WriteIO(12,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Attack1_Out->setText(str00);
}

void MainWindow::on_Decay1_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack1->value() & 0xf) << 4) | (ui->Decay1->value() & 0xf);
    sid->WriteIO(12,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Decay1_Out->setText(str00);
}

void MainWindow::on_Sustain1_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain1->value() & 0xf) << 4) | (ui->Release1->value() & 0xf);
    sid->WriteIO(13,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Sustain1_Out->setText(str00);
}

void MainWindow::on_Release1_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain1->value() & 0xf) << 4) | (ui->Release1->value() & 0xf);
    sid->WriteIO(13,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Release1_Out->setText(str00);
}

void MainWindow::on_Attack2_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack2->value() & 0xf) << 4) | (ui->Decay2->value() & 0xf);
    sid->WriteIO(19,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Attack2_Out->setText(str00);
}

void MainWindow::on_Decay2_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack2->value() & 0xf) << 4) | (ui->Decay2->value() & 0xf);
    sid->WriteIO(19,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Decay2_Out->setText(str00);
}

void MainWindow::on_Sustain2_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain2->value() & 0xf) << 4) | (ui->Release2->value() & 0xf);
    sid->WriteIO(20,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Sustain2_Out->setText(str00);
}

void MainWindow::on_Release2_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain2->value() & 0xf) << 4) | (ui->Release2->value() & 0xf);
    sid->WriteIO(20,wert);

    char str00[32];
    sprintf(str00,"%1.1X",value & 0xf);
    ui->Release2_Out->setText(str00);
}
