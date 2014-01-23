#include "settingsdialog.hpp"
#include "ui_settingsdialog.h"

settingsDialog::settingsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);
}

settingsDialog::~settingsDialog()
{
    delete ui;
}
