#include "mycanvas.h"

myCanvas::myCanvas(QWidget *parent, const QPoint &position, const QSize &size)
    : QSFMLCanvas(parent, position, size)
{
    
}

void myCanvas::onInit()
{
    
}

void myCanvas::onUpdate()
{
    // clear the screen
    Clear(sf::Color(0,128,0));
    
}
