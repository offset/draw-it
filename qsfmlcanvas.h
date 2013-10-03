#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <QWidget>
#include <QTimer>
#include <SFML/Graphics.hpp>

class QSFMLCanvas : public QWidget,
        sf::RenderWindow
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor for the widget
     * \param parent[in]: Defines the parent widget.
     * \param position[in]: The starting position.
     * \param size[in]: The starting size.
     * \param frameTime{in]: The frame time, default 0 is fine.
     */
    explicit QSFMLCanvas(QWidget *parent, 
                         const QPoint& position, 
                         const QSize& size,
                         unsigned int frameTime = 0);
    virtual ~QSFMLCanvas();
    
private:
    /*!
     * \brief Initializes the widget.
     */
    virtual void onInit();
    virtual void onUpdate();
    /*!
     * \brief Tells Qt that we are not goint to use it's built in painting engine by returning 0.
     * \return A nullptr.
     */
    virtual QPaintEngine* paintEngine() const;
    virtual void showEvent(QShowEvent *);
    /*!
     * \brief Notifies the the widget that it is to be refreshed and displays the refreshed frame.
     */
    virtual void paintEvent(QPaintEvent *);
    
    QTimer myTimer;
    bool myInitialized;
    
signals:
    
public slots:
    
};

#endif // QSFMLCANVAS_H
