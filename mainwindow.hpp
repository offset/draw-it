#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "errcodes.hpp"
#include "linefinder.hpp"
#include "play.hpp"
#include <QMessageBox>
#include "settingsdialog.hpp"
#include "selectplayersprite.h"

/*! This namespaces contains the GUI (Ui = user interface). */

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
    
    /*!
     * \brief Executes when the 'select' button is clicked. 
     *
     * Lets the user select a file via a file dialog (provided by Qt), tells the 
     * LineFinder object about it and displays it in the GUI.
     *
     */
    void on_selection_clicked();
    
    /*!
     * \brief Executes when the 'convert' button is clicked.
     *
     * First makes sure there has been an image selected, when not, it aborts. 
     * Then the magic happens, the lines are extracted, the information about their 
     * locations is stored in the std::vector<std::vector<int> > levelMap and at last
     * the extracted lines are displayed in the GUI.
     */
    void on_conversion_clicked();
    
    /*!
     * \brief Executes when the 'play' button is clicked.
     *
     * Makes sure there has been an image selected AND that it has been converted, 
     * then builds a level out of the calculated data and creates the game.
     */
    void on_game_clicked();
    
    /*!
     * \brief When the 'exit' button is clicked this function is executed and 
     * terminates the application.
     */
    void on_exit_clicked();
    
    void on_action_about_triggered();
    
    void on_action_Configure_image_analysis_settings_triggered();
    
    void on_actionSelect_player_sprite_triggered();
    
private:
    Ui::MainWindow *ui;
    settingsDialog sD;
    selectPlayerSprite sPS;
    QWidget about;
    QWidget configImage;
    bool selected;
    bool converted;
    QMessageBox message;
};

#endif // MAINWINDOW_H
