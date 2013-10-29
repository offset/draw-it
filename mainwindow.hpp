#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "errcodes.hpp"
#include "linefinder.hpp"
#include "play.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_game_clicked();
    
    void on_selection_clicked();
    
    void on_conversion_clicked();
    
    void on_exit_clicked();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
