#include "selectplayersprite.h"
#include "ui_selectplayersprite.h"
#include "play.hpp"

selectPlayerSprite::selectPlayerSprite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectPlayerSprite)
{
    ui->setupUi(this);
    
    connect(ui->pushButton,SIGNAL(clicked()),this, SLOT(close()));
}

selectPlayerSprite::~selectPlayerSprite()
{
    delete ui;
}

void selectPlayerSprite::on_comboBox_activated(const QString &arg1)
{
   Play::getInstance()->setPlayerTexture(arg1.toStdString());
}

void selectPlayerSprite::on_pushButton_clicked()
{
    
}
