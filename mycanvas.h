#ifndef MYCANVAS_H
#define MYCANVAS_H
#include <qsfmlcanvas.h>

class myCanvas : public QSFMLCanvas
{
public:
    myCanvas(QWidget* parent,
             const QPoint& position,
             const QSize& size);
    
private:
    void onInit();
    void onUpdate();
};

#endif // MYCANVAS_H
