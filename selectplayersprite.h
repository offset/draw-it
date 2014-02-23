#ifndef SELECTPLAYERSPRITE_H
#define SELECTPLAYERSPRITE_H

#include <QWidget>

namespace Ui {
class selectPlayerSprite;
}

class selectPlayerSprite : public QWidget
{
    Q_OBJECT
    
public:
    explicit selectPlayerSprite(QWidget *parent = 0);
    ~selectPlayerSprite();
    
private slots:
    void on_comboBox_activated(const QString &arg1);
    
private:
    Ui::selectPlayerSprite *ui;
};

#endif // SELECTPLAYERSPRITE_H
