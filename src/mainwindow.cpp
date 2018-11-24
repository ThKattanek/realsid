//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: mainwindow.cpp                        //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 03.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SAMPLERATE 48000
#define PUFFERSIZE 1920

#define WaveOutXW 320
#define WaveOutYW 150

void AudioMix(void *userdata, Uint8 *stream, int laenge);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    WaveOut(NULL),
    NoDrawingWaveOut(true),
    AktSID(0)
{
    ui->setupUi(this);

    setWindowTitle(windowTitle() + " -  Version: " + VERSION_STRING);
    
    int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if(ret < 0)
    {
        QMessageBox::critical(0,"SDL Fehler","Fehler beim Installieren von SDL.");
        QApplication::exit(-1);
        return;
    }

    for(int i=0;i<SID_ANZAHL;i++)
    {
        sid[i] = new SIDClass(SAMPLERATE,PUFFERSIZE);
        SidPuffer[i] = sid[i]->GetSoundPuffer();
    }

    seq = new SequenzerClass();

    sequenzer_win = new SequenzerWindow(this,seq);

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
    sequenzer_win->close();
    delete sequenzer_win;

    ShowWaveOut(false);
    SDL_CloseAudio();

    for(int i=0;i<SID_ANZAHL;i++)
        delete sid[i];
    delete ui;
}

void AudioMix(void *userdata, Uint8 *stream, int laenge)
{
    MainWindow *main = (MainWindow*)userdata;
    main->AudioLoop((short*)stream,laenge);
}

void MainWindow::AudioLoop(short* stream, int laenge)
{
    for(int i=0;i<SID_ANZAHL;i++)
        sid[i]->ResetSoundPufferPos();

    unsigned short seq_ret;

    while(sid[0]->GetSoundPufferPos() < (laenge/2))
    {
        seq_ret = seq->OneCycle();
        if((seq_ret & 0x1f) != 0x1f)
        {
            if((seq_ret>>13)<SID_ANZAHL)
                sid[seq_ret>>13]->WriteIO((seq_ret>>8) & 0x1f,seq_ret & 0xff);
        }

        for(int i=0;i<SID_ANZAHL;i++)
            sid[i]->OneCycle();
    }

    unsigned short *SoundPuffer = (unsigned short*)stream;

    for(int i=0; i<(laenge/2);i++)
    {
        float wave_summe = SidPuffer[0][i];
        for(int j=1;j<SID_ANZAHL;j++) wave_summe += SidPuffer[j][i];
        wave_summe /= SID_ANZAHL;

        SoundPuffer[i] = (unsigned short)(wave_summe* 1.5f * 0x7FFF);
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

    int OldY = SidPuffer[AktSID][0]*-1.5f * WaveOutYW/2 + WaveOutYW/2;
    for (int i=0;i<WaveOutXW;i++)
    {
        AktY = SidPuffer[AktSID][(int)puffer_pos]*-1.5f * WaveOutYW/2 + WaveOutYW/2;
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
    sid[AktSID]->WriteIO(0,wert);
}

void MainWindow::on_Freq0Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq0Hi_Out->setText(str00);
    sid[AktSID]->WriteIO(1,wert);
}

void MainWindow::on_Freq1Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq1Lo_Out->setText(str00);
    sid[AktSID]->WriteIO(7,wert);
}

void MainWindow::on_Freq1Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq1Hi_Out->setText(str00);
    sid[AktSID]->WriteIO(8,wert);
}

void MainWindow::on_Freq2Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq2Lo_Out->setText(str00);
    sid[AktSID]->WriteIO(14,wert);
}

void MainWindow::on_Freq2Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Freq2Hi_Out->setText(str00);
    sid[AktSID]->WriteIO(15,wert);
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
        sid[AktSID]->WriteIO(4,ctrl);
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
        sid[AktSID]->WriteIO(11,ctrl);
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
        sid[AktSID]->WriteIO(18,ctrl);
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
        if(!sid[AktSID]->OpenSIDDump(filename.toAscii().data()))
            QMessageBox::warning(this,"realSID Error !","Fehler beim öffnen des SID Dump Files.");
    }
}

void MainWindow::on_Play_SidDump_triggered()
{
    sid[AktSID]->PlaySIDDump();
}

void MainWindow::on_Stop_SidDump_triggered()
{
    sid[AktSID]->StopSIDDump();
}

void MainWindow::on_Puls0Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls0Lo_Out->setText(str00);
    sid[AktSID]->WriteIO(2,wert);
}

void MainWindow::on_Puls0Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls0Hi_Out->setText(str00);
    sid[AktSID]->WriteIO(3,wert);
}

void MainWindow::on_Puls1Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls1Lo_Out->setText(str00);
    sid[AktSID]->WriteIO(9,wert);
}

void MainWindow::on_Puls1Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls1Hi_Out->setText(str00);
    sid[AktSID]->WriteIO(10,wert);
}

void MainWindow::on_Puls2Lo_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls2Lo_Out->setText(str00);
    sid[AktSID]->WriteIO(16,wert);
}

void MainWindow::on_Puls2Hi_valueChanged(int value)
{
    unsigned char wert = value;

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",wert,wert);
    ui->Puls2Hi_Out->setText(str00);
    sid[AktSID]->WriteIO(17,wert);
}

void MainWindow::on_Attack0_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack0->value() & 0xf) << 4) | (ui->Decay0->value() & 0xf);
    sid[AktSID]->WriteIO(5,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Attack0_Out->setText(str00);
}

void MainWindow::on_Decay0_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack0->value() & 0xf) << 4) | (ui->Decay0->value() & 0xf);
    sid[AktSID]->WriteIO(5,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Decay0_Out->setText(str00);
}

void MainWindow::on_Sustain0_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain0->value() & 0xf) << 4) | (ui->Release0->value() & 0xf);
    sid[AktSID]->WriteIO(6,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Sustain0_Out->setText(str00);
}

void MainWindow::on_Release0_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain0->value() & 0xf) << 4) | (ui->Release0->value() & 0xf);
    sid[AktSID]->WriteIO(6,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Release0_Out->setText(str00);
}

void MainWindow::on_Attack1_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack1->value() & 0xf) << 4) | (ui->Decay1->value() & 0xf);
    sid[AktSID]->WriteIO(12,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Attack1_Out->setText(str00);
}

void MainWindow::on_Decay1_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack1->value() & 0xf) << 4) | (ui->Decay1->value() & 0xf);
    sid[AktSID]->WriteIO(12,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Decay1_Out->setText(str00);
}

void MainWindow::on_Sustain1_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain1->value() & 0xf) << 4) | (ui->Release1->value() & 0xf);
    sid[AktSID]->WriteIO(13,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Sustain1_Out->setText(str00);
}

void MainWindow::on_Release1_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain1->value() & 0xf) << 4) | (ui->Release1->value() & 0xf);
    sid[AktSID]->WriteIO(13,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Release1_Out->setText(str00);
}

void MainWindow::on_Attack2_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack2->value() & 0xf) << 4) | (ui->Decay2->value() & 0xf);
    sid[AktSID]->WriteIO(19,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Attack2_Out->setText(str00);
}

void MainWindow::on_Decay2_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Attack2->value() & 0xf) << 4) | (ui->Decay2->value() & 0xf);
    sid[AktSID]->WriteIO(19,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Decay2_Out->setText(str00);
}

void MainWindow::on_Sustain2_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain2->value() & 0xf) << 4) | (ui->Release2->value() & 0xf);
    sid[AktSID]->WriteIO(20,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Sustain2_Out->setText(str00);
}

void MainWindow::on_Release2_valueChanged(int value)
{
    unsigned char wert;
    wert = ((ui->Sustain2->value() & 0xf) << 4) | (ui->Release2->value() & 0xf);
    sid[AktSID]->WriteIO(20,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Release2_Out->setText(str00);
}

void MainWindow::on_FilterFreq_valueChanged(int value)
{
    sid[AktSID]->WriteIO(21,value & 0x07);
    sid[AktSID]->WriteIO(22,value >> 3);

    char str00[32];
    sprintf(str00,"0x%3.3X[%d]",value & 0x7ff,value & 0x7ff);
    ui->FilterFreq_Out->setText(str00);
}

void MainWindow::on_FilterReso_valueChanged(int value)
{
    unsigned char wert = ui->FilterReso->value() << 4;
    if(ui->FilterV0->isChecked()) wert |= 1;
    if(ui->FilterV1->isChecked()) wert |= 2;
    if(ui->FilterV2->isChecked()) wert |= 4;
    sid[AktSID]->WriteIO(23,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->FilterReso_Out->setText(str00);
}

void MainWindow::on_Volume_valueChanged(int value)
{
    unsigned char wert = ui->Volume->value() & 0x0F;
    if(ui->Tiefpass->isChecked()) wert |= 16;
    if(ui->Bandpass->isChecked()) wert |= 32;
    if(ui->Hochpass->isChecked()) wert |= 64;
    if(ui->MuteVoice2->isChecked()) wert |= 128;
    sid[AktSID]->WriteIO(24,wert);

    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value & 0xf,value & 0xf);
    ui->Volume_Out->setText(str00);
}

void MainWindow::on_FilterV0_clicked()
{
    unsigned char wert = ui->FilterReso->value() << 4;
    if(ui->FilterV0->isChecked()) wert |= 1;
    if(ui->FilterV1->isChecked()) wert |= 2;
    if(ui->FilterV2->isChecked()) wert |= 4;
    sid[AktSID]->WriteIO(23,wert);
}

void MainWindow::on_FilterV1_clicked()
{
    unsigned char wert = ui->FilterReso->value() << 4;
    if(ui->FilterV0->isChecked()) wert |= 1;
    if(ui->FilterV1->isChecked()) wert |= 2;
    if(ui->FilterV2->isChecked()) wert |= 4;
    sid[AktSID]->WriteIO(23,wert);
}

void MainWindow::on_FilterV2_clicked()
{
    unsigned char wert = ui->FilterReso->value() << 4;
    if(ui->FilterV0->isChecked()) wert |= 1;
    if(ui->FilterV1->isChecked()) wert |= 2;
    if(ui->FilterV2->isChecked()) wert |= 4;
    sid[AktSID]->WriteIO(23,wert);
}

void MainWindow::on_Tiefpass_clicked()
{
    unsigned char wert = ui->Volume->value() & 0x0F;
    if(ui->Tiefpass->isChecked()) wert |= 16;
    if(ui->Bandpass->isChecked()) wert |= 32;
    if(ui->Hochpass->isChecked()) wert |= 64;
    if(ui->MuteVoice2->isChecked()) wert |= 128;
    sid[AktSID]->WriteIO(24,wert);
}

void MainWindow::on_Hochpass_clicked()
{
    unsigned char wert = ui->Volume->value() & 0x0F;
    if(ui->Tiefpass->isChecked()) wert |= 16;
    if(ui->Bandpass->isChecked()) wert |= 32;
    if(ui->Hochpass->isChecked()) wert |= 64;
    if(ui->MuteVoice2->isChecked()) wert |= 128;
    sid[AktSID]->WriteIO(24,wert);
}

void MainWindow::on_Bandpass_clicked()
{
    unsigned char wert = ui->Volume->value() & 0x0F;
    if(ui->Tiefpass->isChecked()) wert |= 16;
    if(ui->Bandpass->isChecked()) wert |= 32;
    if(ui->Hochpass->isChecked()) wert |= 64;
    if(ui->MuteVoice2->isChecked()) wert |= 128;
    sid[AktSID]->WriteIO(24,wert);
}

void MainWindow::on_MuteVoice2_clicked()
{
    unsigned char wert = ui->Volume->value() & 0x0F;
    if(ui->Tiefpass->isChecked()) wert |= 16;
    if(ui->Bandpass->isChecked()) wert |= 32;
    if(ui->Hochpass->isChecked()) wert |= 64;
    if(ui->MuteVoice2->isChecked()) wert |= 128;
    sid[AktSID]->WriteIO(24,wert);
}

void MainWindow::on_actionSequenzer_triggered()
{
    if(sequenzer_win->isHidden()) sequenzer_win->show();
    else sequenzer_win->hide();
}
