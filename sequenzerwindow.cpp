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
    BPM->setValue(230);
    seq->SetBPM(230);
    ui->toolBar->addWidget(BPM);

    /// Songlänge
    ui->toolBar->addWidget(new QLabel(" Songlänge: "));
    SONGSIZE = new QSpinBox(this);
    connect(SONGSIZE, SIGNAL(valueChanged(int)),this,SLOT(onSongLengthChange(int)));
    SONGSIZE->setMaximum(MAX_PATTERN-1);
    SONGSIZE->setMinimum(0);
    SONGSIZE->setValue(1);
    seq->SetSongLength(1);
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

    /// StepTable formatieren
    ui->StepTable->setRowCount(MAX_STEPS);
    ui->StepTable->setColumnCount(3);
    ui->StepTable->setColumnWidth(0,50);
    ui->StepTable->setColumnWidth(1,50);
    ui->StepTable->setColumnWidth(2,50);

    /// StepTablelistheader mit Überschriften füllen
    list<<"Voice1"<<"Voice2"<<"Voice3";
    ui->StepTable->setHorizontalHeaderLabels(list);

    /// StepTable Zellen mit entspr. Widget füllen
    for(int i=0;i<MAX_STEPS;i++)
    {
        ui->StepTable->setRowHeight(i,17);

        SpinBox *spin1 = new SpinBox(i);
        spin1->setMaximum(MAX_PATTERN-1);
        spin1->setFrame(false);
        spin1->setButtonSymbols(QAbstractSpinBox::NoButtons);

        connect(spin1,SIGNAL(ChangeValue(int,int)),this,SLOT(onV1StepChange(int,int)));
        ui->StepTable->setCellWidget(i,0,spin1);

        SpinBox *spin2 = new SpinBox(i);
        spin2->setMaximum(MAX_PATTERN-1);
        spin2->setFrame(false);
        spin2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        connect(spin2,SIGNAL(ChangeValue(int,int)),this,SLOT(onV2StepChange(int,int)));
        ui->StepTable->setCellWidget(i,1,spin2);

        SpinBox *spin3 = new SpinBox(i);
        spin3->setMaximum(MAX_PATTERN-1);
        spin3->setFrame(false);
        spin3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        connect(spin3,SIGNAL(ChangeValue(int,int)),this,SLOT(onV3StepChange(int,int)));
        ui->StepTable->setCellWidget(i,2,spin3);
    }

    /// Patterntabelle formatieren
    ui->PatternTable->setRowCount(PATTERN_LEN);
    ui->PatternTable->setColumnCount(2);
    ui->PatternTable->setColumnWidth(0,50);
    ui->PatternTable->setColumnWidth(1,60);

    /// Patternlistheader mit Überschriften füllen
    list<<"Noten"<<"SoundNr";
    ui->PatternTable->setHorizontalHeaderLabels(list);

    /// Patternlist Zellen mit entspr. Widget füllen
    for(int i=0;i<PATTERN_LEN;i++)
    {
        ui->PatternTable->setRowHeight(i,20);
        SpinBox *spin = new SpinBox(i);
        spin->setMaximum(255);
        connect(spin,SIGNAL(ChangeValue(int,int)),this,SLOT(onSoundNrChange(int,int)));
        ui->PatternTable->setCellWidget(i,1,spin);

        NotenEdit *notenedit = new NotenEdit(i);
        connect(notenedit,SIGNAL(ChangeNote(int,int)),this,SLOT(onNoteChange(int,int)));
        ui->PatternTable->setCellWidget(i,0,notenedit);
    }

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
}

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

    ui->PatternNr->setValue(0);
    FillPatternList();
    ui->SoundNr->setValue(0);
    FillSoundFrame();
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

void SequenzerWindow::onV1StepChange(int id, int value)
{
    StepTable[id].Track[0].PatterNr = value;
}

void SequenzerWindow::onV2StepChange(int id, int value)
{
    StepTable[id].Track[1].PatterNr = value;
}

void SequenzerWindow::onV3StepChange(int id, int value)
{
    StepTable[id].Track[2].PatterNr = value;
}

void SequenzerWindow::FillPatternList()
{
    /// PatternListe mit Werte aus Sequenzer füllen
    NotenEdit *noten_edit;
    SpinBox *soundnr_edit;

    for(int i=0;i<PATTERN_LEN;i++)
    {
        noten_edit = (NotenEdit*)ui->PatternTable->cellWidget(i,0);
        noten_edit->SetNote(AktPattern->Note[i]);

        soundnr_edit = (SpinBox*)ui->PatternTable->cellWidget(i,1);
        soundnr_edit->setValue(AktPattern->SoundNr[i]);
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
