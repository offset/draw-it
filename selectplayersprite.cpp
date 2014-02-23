#include "selectplayersprite.h"
#include "ui_selectplayersprite.h"
#include "play.hpp"

selectPlayerSprite::selectPlayerSprite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectPlayerSprite)
{
    ui->setupUi(this);
}

selectPlayerSprite::~selectPlayerSprite()
{
    delete ui;
}

void selectPlayerSprite::on_comboBox_activated(const QString &arg1)
{
   Play::getInstance()->setPlayerTexture(arg1.toStdString());
}
