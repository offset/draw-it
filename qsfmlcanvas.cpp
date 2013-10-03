#include "qsfmlcanvas.h"

QSFMLCanvas::QSFMLCanvas(QWidget *parent, 
                         const QPoint &position, 
                         const QSize &size, 
                         unsigned int frameTime) :
    QWidget(parent), myInitialized(false)
{
    // set some attributes to enable direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    
    // this enables keyboard events to be sent to the widget
    setFocusPolicy(Qt::StrongFocus);
    
    // set the geometry
    move(position);
    resize(size);
    
    // setup the timer
    myTimer.setInterval(frameTime);
}

// needed when the application runs under a system using X11 (linux, os x, etc);
#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

void QSFMLCanvas::showEvent(QShowEvent *)
{
    if(!myInitialized)
    {
    // we need to flush the output buffer (commands sent to the xserver) to make sure that
    // SFML gets an updated view of the windows
    #ifdef Q_WS_X11
        XFlush(QX11Info::display());
    #endif
        
    // we create the SFML window
    Create(winId());
    
    // we initialize the window
    OnInit();
    
    // sets the timer to let the window be refreshed everytime the timer says timeout and resets it
    connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
    myTimer.start();
    
    myInitialized = true;
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}

void QSFMLCanvas::paintEvent(QPaintEvent *)
{
    OnUpdate();
    Display();
}
