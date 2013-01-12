#ifndef SPINBOX_H
#define SPINBOX_H

#include <QSpinBox>

class SpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit SpinBox(int id, QWidget *parent = 0);

signals:
    void ChangeValue(int id, int value);
public slots:
    void onvalueChanged(int);
private:
    int ID;
};

#endif // SPINBOX_H
