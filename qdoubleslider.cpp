#include "qdoubleslider.h"

QDoubleSlider::QDoubleSlider(QWidget *parent) :
    QSlider(parent)
{
}

void QDoubleSlider::setDoubleValue(double value)
{
    setValue(static_cast<int>(value));
}
