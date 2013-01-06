#ifndef SEQUENZERWINDOW_H
#define SEQUENZERWINDOW_H

#include <QMainWindow>
#include <sequenzerclass.h>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

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

private:
    Ui::SequenzerWindow *ui;
    SequenzerClass *seq;
    QSpinBox *BPM;
    QPushButton *PlaySong;
    QPushButton *StopSong;
};

#endif // SEQUENZERWINDOW_H
