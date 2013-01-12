#include "sequenzerwindow.h"
#include "ui_sequenzerwindow.h"

SequenzerWindow::SequenzerWindow(QWidget *parent, SequenzerClass *_seq) :
    QMainWindow(parent),
    ui(new Ui::SequenzerWindow)
{
    seq = _seq;
    ui->setupUi(this);

    ui->toolBar->addWidget(new QLabel("bpm"));

    BPM = new QSpinBox(this);
    connect(BPM, SIGNAL(valueChanged(int)),this,SLOT(onBPMChange(int)));
    BPM->setMaximum(999);
    BPM->setMinimum(0);
    BPM->setValue(120);
    seq->SetBPM(120);
    ui->toolBar->addWidget(BPM);

    PlaySong = new QPushButton("Play");
    connect(PlaySong,SIGNAL(clicked()),this,SLOT(onPlaySongClick()));
    ui->toolBar->addWidget(PlaySong);
    StopSong = new QPushButton("Stop");
    connect(StopSong,SIGNAL(clicked()),this,SLOT(onStopSongClick()));
    ui->toolBar->addWidget(StopSong);

    ui->PatternTable->setRowCount(PATTERN_LEN);
    ui->PatternTable->setColumnCount(2);
    ui->PatternTable->setColumnWidth(0,40);
    ui->PatternTable->setColumnWidth(1,50);

    list<<"Noten"<<"SoundNr";
    ui->PatternTable->setHorizontalHeaderLabels(list);

    for(int i=0;i<PATTERN_LEN;i++)
    {
        ui->PatternTable->setRowHeight(i,24);
        SpinBox *spin = new SpinBox(i);
        spin->setMaximum(255);
        connect(spin,SIGNAL(ChangeValue(int,int)),this,SLOT(onSoundNrChange(int,int)));
        ui->PatternTable->setCellWidget(i,1,spin);

        NotenEdit *notenedit = new NotenEdit(i);
        connect(notenedit,SIGNAL(ChangeNote(int,int)),this,SLOT(onNoteChange(int,int)));
        ui->PatternTable->setCellWidget(i,0,notenedit);

    }

    ui->PatternNr->setMaximum(MAX_PATTERN-1);
    ui->PatternNr->setValue(0);
    AktPattern = seq->GetPatternPointer(0);
    FillPatternList();
}

SequenzerWindow::~SequenzerWindow()
{
    delete ui;
}

void SequenzerWindow::onBPMChange(int value)
{
    seq->SetBPM(value);
}

void SequenzerWindow::onPlaySongClick()
{
    seq->Play();
}
void SequenzerWindow::onStopSongClick()
{
    seq->Stop();
}

void SequenzerWindow::on_PatternNr_valueChanged(int value)
{
    AktPattern = seq->GetPatternPointer(value);
    FillPatternList();
}

void SequenzerWindow::onSoundNrChange(int id, int value)
{
    //QMessageBox::information(this,QVariant(id).toString(),QVariant(value).toString());
}

void SequenzerWindow::onNoteChange(int id, int noten_nr)
{
}

void SequenzerWindow::FillPatternList()
{
   // ui->PatternTable->item(0,0)->setText("a");
    /*
    for(int i=0;i<PATTERN_LEN;i++)
    {
        //ui->PatternTable->item(1,i)->setText(QVariant(AktPattern->SoundNr[i]).toString());
        ui->PatternTable->item(i,0)->setText("a");
    }
    */
}
