#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <iostream>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // makes the exit button work
    connect(ui->exit,SIGNAL(clicked()),this, SLOT(close()));
    
    selected = false;
    converted = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selection_clicked()
{
    // when this button is clicked, the user may choose an image which is set as the to be processed image
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose Image"), QDesktopServices::storageLocation(QDesktopServices::HomeLocation), tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
    Play::getInstance()->getFinder()->setImage(filename.toLatin1().data());
    // The selected image is being displayed in a label in the gui
    QImage img = QImage(static_cast<unsigned char*>(Play::getInstance()->getFinder()->getImage().data), 
                                                    Play::getInstance()->getFinder()->getImage().cols,
                                                    Play::getInstance()->getFinder()->getImage().rows,
                                                    QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->label->resize(ui->label->pixmap()->size());
    
    selected = true;
}

void MainWindow::on_conversion_clicked()
{
    // makes sure that there has been an image previously selected
    if(!selected)
    {
        message.setText("Please select an image first!");
        message.exec();
        return;
    }
    // The lines are being detected and an image with the lines only is being displayed
    // It is tried again and again until there were lines detected
    int errorSuccess = 0;
    int loopCnt = 0;
    while (errorSuccess == 0)
    {
        if (Play::getInstance()->detect() != -1)
        {
            errorSuccess = 1;
        }
        if (loopCnt == 20)
        {
            std::cerr << "There could be no lines detected after 20 loop runs." << std::endl
                      << "Please try another image." << std::endl;
            message.setText("Please try again or provide another image.");
            exit(-1);
        }
    }
    cv::Mat image;
    // The image needs to be converted to BGR to be displayed correctly by the Qt window.
    cv::cvtColor(Play::getInstance()->getFinder()->getImage(), Play::getInstance()->getFinder()->getImage(), cv::COLOR_GRAY2BGR);
    QImage img = QImage(static_cast<unsigned char*>(Play::getInstance()->getFinder()->getImage().data), 
                                                    Play::getInstance()->getFinder()->getImage().cols,
                                                    Play::getInstance()->getFinder()->getImage().rows,
                                                    QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->label->resize(ui->label->pixmap()->size());
    
    converted = true;
}

void MainWindow::on_game_clicked()
{
    if(!selected)
    {
        message.setText("Please select an image first!");
        message.exec();
        return;
    }
    if(!converted)
    {
        message.setText("Please convert the selected image first!");
        message.exec();
        return;
    }
    if(Play::getInstance()->getLevelMap().empty())
    {
        std::cout << "There were no lines detected." << std::endl
                  << "Now exiting." << std::endl;
        exit(-1);
    }
    Play::getInstance()->play();
}

void MainWindow::on_exit_clicked()
{
    // see constructor
}
