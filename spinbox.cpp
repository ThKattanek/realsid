#include "spinbox.h"

SpinBox::SpinBox(int id, QWidget *parent) :
    QSpinBox(parent)
{
    ID = id;

    connect(this,SIGNAL(valueChanged(int)),this,SLOT(onvalueChanged(int)));
}

void SpinBox::onvalueChanged(int value)
{
    emit ChangeValue(ID,value);
}
