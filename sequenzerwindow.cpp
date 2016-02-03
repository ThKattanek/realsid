//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: sequenzerwindow.cpp                   //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 12.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#include "sequenzerwindow.h"
#include "ui_sequenzerwindow.h"

SequenzerWindow::SequenzerWindow(QWidget *parent, SequenzerClass *_seq) :
    QMainWindow(parent),
    ui(new Ui::SequenzerWindow)
{
    seq = _seq;
    ui->setupUi(this);

    StepTable = seq->GetStepTablePointer();

    /// Toolbar mit Steuerelente füllen
    ui->toolBar->addWidget(new QLabel("BPM: "));

    /// Speedeinstellung
    BPM = new QSpinBox(this);
    connect(BPM, SIGNAL(valueChanged(int)),this,SLOT(onBPMChange(int)));
    BPM->setMaximum(999);
    BPM->setMinimum(0);
    ui->toolBar->addWidget(BPM);

    /// Songlänge
    ui->toolBar->addWidget(new QLabel(" Songlänge: "));
    SONGSIZE = new QSpinBox(this);
    connect(SONGSIZE, SIGNAL(valueChanged(int)),this,SLOT(onSongLengthChange(int)));
    SONGSIZE->setMaximum(MAX_PATTERN-1);
    SONGSIZE->setMinimum(0);
    ui->toolBar->addWidget(SONGSIZE);

    /// Play Button
    PlaySong = new QPushButton("Play");
    connect(PlaySong,SIGNAL(clicked()),this,SLOT(onPlaySongClick()));
    ui->toolBar->addWidget(PlaySong);

    /// Stop Button
    StopSong = new QPushButton("Stop");
    connect(StopSong,SIGNAL(clicked()),this,SLOT(onStopSongClick()));
    ui->toolBar->addWidget(StopSong);

    /// Clear Button
    ClearSong = new QPushButton("New");
    connect(ClearSong,SIGNAL(clicked()),this,SLOT(onClearSongClick()));
    ui->toolBar->addWidget(ClearSong);

    /// Load Button
    LoadSong = new QPushButton("Load Song");
    connect(LoadSong,SIGNAL(clicked()),this,SLOT(onLoadSongClick()));
    ui->toolBar->addWidget(LoadSong);

    /// Save Button
    SaveSong = new QPushButton("Save Song");
    connect(SaveSong,SIGNAL(clicked()),this,SLOT(onSaveSongClick()));
    ui->toolBar->addWidget(SaveSong);

    /// StepTable formatieren
    ui->StepTable->setRowCount(MAX_STEPS);
    ui->StepTable->setColumnCount(6);
    for(int i=0;i<6;i++)
        ui->StepTable->setColumnWidth(i,46);

    /// StepTablelistheader mit Überschriften füllen
    list0<<"V1 PA"<<"V1 TR"<<"V2 PA"<<"V2 TR"<<"V3 PA"<<"V3 TR";
    ui->StepTable->setHorizontalHeaderLabels(list0);

    /// StepTable Zellen mit entspr. Widget füllen
    for(int i=0;i<MAX_STEPS;i++)
    {
        ui->StepTable->setRowHeight(i,17);

        SpinBox *spin1 = new SpinBox(i);
        spin1->setMaximum(MAX_PATTERN-1);
        spin1->setFrame(false);
        spin1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin1->setStyleSheet("background-color: rgb(203, 199, 255);");
        connect(spin1,SIGNAL(ChangeValue(int,int)),this,SLOT(onV1StepPatternChange(int,int)));
        ui->StepTable->setCellWidget(i,0,spin1);

        spin1 = new SpinBox(i);
        spin1->setMaximum(63);
        spin1->setMinimum(-63);
        spin1->setValue(0);
        spin1->setFrame(false);
        spin1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin1->setStyleSheet("background-color: rgb(203, 199, 255);");
        connect(spin1,SIGNAL(ChangeValue(int,int)),this,SLOT(onV1StepTransposeChange(int,int)));
        ui->StepTable->setCellWidget(i,1,spin1);

        spin1 = new SpinBox(i);
        spin1->setMaximum(MAX_PATTERN-1);
        spin1->setFrame(false);
        spin1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin1->setStyleSheet("background-color: rgb(180, 180, 255);");
        connect(spin1,SIGNAL(ChangeValue(int,int)),this,SLOT(onV2StepPatternChange(int,int)));
        ui->StepTable->setCellWidget(i,2,spin1);

        spin1 = new SpinBox(i);
        spin1->setMaximum(63);
        spin1->setMinimum(-63);
        spin1->setValue(0);
        spin1->setFrame(false);
        spin1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin1->setStyleSheet("background-color: rgb(180, 180, 255);");
        connect(spin1,SIGNAL(ChangeValue(int,int)),this,SLOT(onV2StepTransposeChange(int,int)));
        ui->StepTable->setCellWidget(i,3,spin1);

        spin1 = new SpinBox(i);
        spin1->setMaximum(MAX_PATTERN-1);
        spin1->setFrame(false);
        spin1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin1->setStyleSheet("background-color: rgb(200, 200, 255);");
        connect(spin1,SIGNAL(ChangeValue(int,int)),this,SLOT(onV3StepPatternChange(int,int)));
        ui->StepTable->setCellWidget(i,4,spin1);

        spin1 = new SpinBox(i);
        spin1->setMaximum(63);
        spin1->setMinimum(-63);
        spin1->setValue(0);
        spin1->setFrame(false);
        spin1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin1->setStyleSheet("background-color: rgb(200, 200, 255);");
        connect(spin1,SIGNAL(ChangeValue(int,int)),this,SLOT(onV3StepTransposeChange(int,int)));
        ui->StepTable->setCellWidget(i,5,spin1);
    }

    /// Patterntabelle formatieren
    ui->PatternTable->setRowCount(PATTERN_LEN);
    ui->PatternTable->setColumnCount(4);
    ui->PatternTable->setColumnWidth(0,50);
    ui->PatternTable->setColumnWidth(1,60);
    ui->PatternTable->setColumnWidth(2,60);
    ui->PatternTable->setColumnWidth(3,80);

    /// Patternlistheader mit Überschriften füllen
    list1<<"Noten"<<"SoundNr"<<"Effekt Nr."<<"Effekt Param.";
    ui->PatternTable->setHorizontalHeaderLabels(list1);

    /// Patternlist Zellen mit entspr. Widget füllen
    for(int i=0;i<PATTERN_LEN;i++)
    {
        ui->PatternTable->setRowHeight(i,20);

        NotenEdit *notenedit = new NotenEdit(i);
        notenedit->setFrame(false);
        connect(notenedit,SIGNAL(ChangeNote(int,int)),this,SLOT(onNoteChange(int,int)));
        ui->PatternTable->setCellWidget(i,0,notenedit);

        SpinBox *spin = new SpinBox(i);
        spin->setMaximum(255);
        spin->setFrame(false);
        connect(spin,SIGNAL(ChangeValue(int,int)),this,SLOT(onSoundNrChange(int,int)));
        ui->PatternTable->setCellWidget(i,1,spin);

        spin = new SpinBox(i);
        spin->setMaximum(255);
        spin->setFrame(false);
        connect(spin,SIGNAL(ChangeValue(int,int)),this,SLOT(onEffektNrChange(int,int)));
        ui->PatternTable->setCellWidget(i,2,spin);

        spin = new SpinBox(i);
        spin->setMaximum(0xFFFF);
        spin->setFrame(false);
        connect(spin,SIGNAL(ChangeValue(int,int)),this,SLOT(onEffektPara1Change(int,int)));
        ui->PatternTable->setCellWidget(i,3,spin);
    }

    /// Songparameter und SteperTable Formatieren
    FillSongParameter();
    FillStepList();

    /// PattenNummer Formatiern
    ui->PatternNr->setMaximum(MAX_PATTERN-1);
    ui->PatternNr->setValue(0);
    AktPattern = seq->GetPatternPointer(0);
    FillPatternList();

    /// SoundNummer Formatieren
    ui->SoundNr->setMaximum(MAX_SOUNDS-1);
    ui->SoundNr->setMinimum(0);
    AktSound = seq->GetSoundPointer(0);
    FillSoundFrame();


    timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), this, SLOT(onTimer1()));
    timer1->start(40);
}

SequenzerWindow::~SequenzerWindow()
{
    delete ui;
}

void SequenzerWindow::onBPMChange(int value)
{
    seq->SetBPM(value);
}

void SequenzerWindow::onSongLengthChange(int value)
{
    seq->SetSongLength(value);
}unsigned short EffektParameter1[PATTERN_LEN];   // Entspr. Effekt Parameter

void SequenzerWindow::onPlaySongClick()
{
    seq->Play();
}

void SequenzerWindow::onStopSongClick()
{
    seq->Stop();
}

void SequenzerWindow::onClearSongClick()
{
    seq->ClearSong();

    FillSongParameter();
    FillStepList();
    ui->PatternNr->setValue(0);
    FillPatternList();
    ui->SoundNr->setValue(0);
    FillSoundFrame();
}

void SequenzerWindow::onLoadSongClick()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("realSID Sequenzerfile öffnen"),"",tr("ealSID Sequenzer Datei") + "(*.seq);;" + tr("Alle Dateien") + "(*.*)");
    if(filename != "")
    {
        if(!seq->LoadSong(filename.toAscii().data())) qDebug() << "Fehler beim Laden";
    }

    FillSongParameter();
    FillStepList();
    FillPatternList();
    FillSoundFrame();
}

void SequenzerWindow::onSaveSongClick()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("realSID Sequenzerfile speichern"),"",tr("ealSID Sequenzer Datei") + "(*.seq);;" + tr("Alle Dateien") + "(*.*)");
    if(filename != "")
    {
        seq->SaveSong(filename.toAscii().data());
    }
}

void SequenzerWindow::onTimer1()
{
    int akt_pattern = seq->GetAktPatternPos();
    int akt_step = seq->GetAktStepPos();

    setWindowTitle("miniSequenzer [STP: " + QVariant(akt_step).toString() + " PAT: " + QVariant(akt_pattern).toString() + "]");
}

void SequenzerWindow::on_PatternNr_valueChanged(int value)
{
    /// Aktuell zu bearbeitende Patternnummer wurde verändert
    AktPattern = seq->GetPatternPointer(value);
    FillPatternList();
}

void SequenzerWindow::onSoundNrChange(int id, int sound_nr)
{
    /// SoundNummer in PatternEdit wurde verändert
    AktPattern->SoundNr[id] = sound_nr;
}

void SequenzerWindow::onNoteChange(int id, int noten_nr)
{
    /// NotenNummer in PatternEdit wurde verändert
    AktPattern->Note[id] = noten_nr;
}

void SequenzerWindow::onEffektNrChange(int id, int value)
{
    /// EffektNummer in PatternEdit wurde verändert
    AktPattern->EffektNr[id] = value;
}

void SequenzerWindow::onEffektPara1Change(int id, int value)
{
    /// EffektParameter in PatternEdit wurde verändert
    AktPattern->EffektParameter1[id] = value;
}

void SequenzerWindow::onV1StepPatternChange(int id, int value)
{
    StepTable[id].Track[0].PatterNr = value;
}

void SequenzerWindow::onV2StepPatternChange(int id, int value)
{
    StepTable[id].Track[1].PatterNr = value;
}

void SequenzerWindow::onV3StepPatternChange(int id, int value)
{
    StepTable[id].Track[2].PatterNr = value;
}

void SequenzerWindow::onV1StepTransposeChange(int id, int value)
{
    StepTable[id].Track[0].Transpose = (char)value;
}

void SequenzerWindow::onV2StepTransposeChange(int id, int value)
{
    StepTable[id].Track[1].Transpose = (char)value;
}

void SequenzerWindow::onV3StepTransposeChange(int id, int value)
{
    StepTable[id].Track[2].Transpose = (char)value;
}

void SequenzerWindow::FillSongParameter()
{
    SONGSIZE->setValue(seq->GetSongLength());
    BPM->setValue(seq->GetBPM());
}

void SequenzerWindow::FillStepList()
{
    SpinBox* sPA;
    SpinBox* sTR;

    /// StepTable Zellen mit entspr. Widgets füllen
    for(int i=0;i<MAX_STEPS;i++)
    {
        sPA = (SpinBox*)ui->StepTable->cellWidget(i,0);
        sTR = (SpinBox*)ui->StepTable->cellWidget(i,1);

        sPA->setValue(StepTable[i].Track[0].PatterNr);
        sTR->setValue(StepTable[i].Track[0].Transpose);

        sPA = (SpinBox*)ui->StepTable->cellWidget(i,2);
        sTR = (SpinBox*)ui->StepTable->cellWidget(i,3);

        sPA->setValue(StepTable[i].Track[1].PatterNr);
        sTR->setValue(StepTable[i].Track[1].Transpose);

        sPA = (SpinBox*)ui->StepTable->cellWidget(i,4);
        sTR = (SpinBox*)ui->StepTable->cellWidget(i,5);

        sPA->setValue(StepTable[i].Track[2].PatterNr);
        sTR->setValue(StepTable[i].Track[2].Transpose);
    }
}

void SequenzerWindow::FillPatternList()
{
    /// PatternListe mit Werte aus Sequenzer füllen
    NotenEdit *noten_edit;
    SpinBox *soundnr_edit;
    SpinBox *effektnr_edit;
    SpinBox *effektpara1_edit;

    for(int i=0;i<PATTERN_LEN;i++)
    {
        noten_edit = (NotenEdit*)ui->PatternTable->cellWidget(i,0);
        noten_edit->SetNote(AktPattern->Note[i]);        

        soundnr_edit = (SpinBox*)ui->PatternTable->cellWidget(i,1);
        soundnr_edit->setValue(AktPattern->SoundNr[i]);

        effektnr_edit = (SpinBox*)ui->PatternTable->cellWidget(i,2);
        effektnr_edit->setValue(AktPattern->EffektNr[i]);

        effektpara1_edit = (SpinBox*)ui->PatternTable->cellWidget(i,3);
        effektpara1_edit->setValue(AktPattern->EffektParameter1[i]);
    }
}

void SequenzerWindow::on_SoundNr_valueChanged(int value)
{
    AktSound = seq->GetSoundPointer(value);
    FillSoundFrame();
}

void SequenzerWindow::FillSoundFrame()
{
    /// SoundFrameSteuerelemente mit Werte aus Sequenzer füllen
    ui->Attack->setValue(AktSound->Attack);
    ui->Decay->setValue(AktSound->Decay);
    ui->Sustain->setValue(AktSound->Sustain);
    ui->Release->setValue(AktSound->Release);
    ui->Pulsweite->setValue(AktSound->Pulsweite);

    on_Attack_valueChanged(AktSound->Attack);
    on_Decay_valueChanged(AktSound->Decay);
    on_Sustain_valueChanged(AktSound->Sustain);
    on_Release_valueChanged(AktSound->Release);
    on_Pulsweite_valueChanged(AktSound->Pulsweite);

    ui->WaveForm->setCurrentIndex(AktSound->Waveform);
}

void SequenzerWindow::on_Attack_valueChanged(int value)
{
    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value, value);
    ui->Attack_Out->setText(str00);
    AktSound->Attack = value;
}

void SequenzerWindow::on_Decay_valueChanged(int value)
{
    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value, value);
    ui->Decay_Out->setText(str00);
    AktSound->Decay = value;
}

void SequenzerWindow::on_Sustain_valueChanged(int value)
{
    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value, value);
    ui->Sustain_Out->setText(str00);
    AktSound->Sustain = value;
}

void SequenzerWindow::on_Release_valueChanged(int value)
{
    char str00[32];
    sprintf(str00,"0x%2.2X[%d]",value, value);
    ui->Release_Out->setText(str00);
    AktSound->Release = value;
}

void SequenzerWindow::on_WaveForm_currentIndexChanged(int index)
{
    AktSound->Waveform = index;
}

void SequenzerWindow::on_Pulsweite_valueChanged(int value)
{
    char str00[32];
    sprintf(str00,"0x%3.3X[%d]",value, value);
    ui->Pulsweite_Out->setText(str00);
    AktSound->Pulsweite = value;
}

void SequenzerWindow::on_ClearPattern_clicked()
{
    for(int i=0;i<PATTERN_LEN;i++)
    {
        AktPattern->Note[i] = 0xff;
        AktPattern->SoundNr[i] = 0;
    }
    FillPatternList();
}

void SequenzerWindow::on_ClearSound_clicked()
{
    AktSound->Attack = 3;
    AktSound->Decay = 3;
    AktSound->Sustain = 15;
    AktSound->Release = 8;
    AktSound->Ring = false;
    AktSound->Pulsweite = 0x7FF;
    AktSound->Waveform = 1;

    FillSoundFrame();
}
