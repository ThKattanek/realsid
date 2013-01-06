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
