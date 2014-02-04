#ifndef QDOUBLESLIDER_H
#define QDOUBLESLIDER_H

#include <QSlider>

class QDoubleSlider : public QSlider
{
    Q_OBJECT
public:
    explicit QDoubleSlider(QWidget *parent = 0);
    
signals:
    
public slots:
    void setDoubleValue(double value);
};

#endif // QDOUBLESLIDER_H
