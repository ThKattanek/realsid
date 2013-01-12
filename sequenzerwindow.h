#ifndef SEQUENZERWINDOW_H
#define SEQUENZERWINDOW_H

#include <QMainWindow>
#include <sequenzerclass.h>
#include <QList>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QVariant>

#include <notenedit.h>
#include <spinbox.h>

namespace Ui {
    class SequenzerWindow;
}

class SequenzerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SequenzerWindow(QWidget *parent = 0, SequenzerClass *_seq = 0);
    ~SequenzerWindow();

private slots:
    void onBPMChange(int value);
    void onPlaySongClick();
    void onStopSongClick();
    void onSoundNrChange(int id, int value);
    void onNoteChange(int id, int value);

    void on_PatternNr_valueChanged(int arg1);

private:
    void FillPatternList(void);
    Ui::SequenzerWindow *ui;
    SequenzerClass *seq;
    QSpinBox *BPM;
    QPushButton *PlaySong;
    QPushButton *StopSong;
    QStringList list;

    PATTERN *AktPattern;
};

#endif // SEQUENZERWINDOW_H
