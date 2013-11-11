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
}

void MainWindow::on_conversion_clicked()
{
    // The lines are being detected and an image with the lines only is being displayed
    Play::getInstance()->detect();
    cv::Mat image;
    cv::cvtColor(Play::getInstance()->getFinder()->getImage(), Play::getInstance()->getFinder()->getImage(), cv::COLOR_GRAY2BGR);
    QImage img = QImage(static_cast<unsigned char*>(Play::getInstance()->getFinder()->getImage().data), 
                                                    Play::getInstance()->getFinder()->getImage().cols,
                                                    Play::getInstance()->getFinder()->getImage().rows,
                                                    QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->label->resize(ui->label->pixmap()->size());
    cv::imwrite("convertedImage.png", Play::getInstance()->getFinder()->getImage());
}

void MainWindow::on_game_clicked()
{
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
    
}
