#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QWidget>
//#include "qdoubleslider.h"

namespace Ui {
class settingsDialog;
}

class settingsDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit settingsDialog(QWidget *parent = 0);
    ~settingsDialog();
    
private slots:
    void on_applyPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::settingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
