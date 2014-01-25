#include "settingsdialog.hpp"
#include "ui_settingsdialog.h"
#include "play.hpp"

settingsDialog::settingsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);

    // Slider and QSpinbox have the same value and each one changes when the other one does
    QObject::connect(ui->minLengthDoubleSpinBox, SIGNAL(valueChanged(int)), ui->minLengthSlider, SLOT(setValue(int)));
    QObject::connect(ui->minLengthSlider, SIGNAL(valueChanged(int)), ui->minLengthDoubleSpinBox, SLOT(setValue(int)));
    ui->minLengthDoubleSpinBox->setValue(Play::getInstance()->getMinLength());

    QObject::connect(ui->minVoteSpinBox, SIGNAL(valueChanged(int)), ui->minVoteSlider, SLOT(setValue(int)));
    QObject::connect(ui->minVoteSlider, SIGNAL(valueChanged(int)), ui->minVoteSpinBox, SLOT(setValue(int)));
    ui->minVoteSpinBox->setValue(Play::getInstance()->getMinVote());

    QObject::connect(ui->maxGapDoubleSpinBox, SIGNAL(valueChanged(int)), ui->maxGapSlider, SLOT(setValue(int)));
    QObject::connect(ui->maxGapSlider, SIGNAL(valueChanged(int)), ui->maxGapDoubleSpinBox, SLOT(setValue(int)));
    ui->maxGapDoubleSpinBox->setValue(Play::getInstance()->getMaxGap());

    QObject::connect(ui->skelThreshSpinBox, SIGNAL(valueChanged(int)), ui->skelThreshSlider, SLOT(setValue(int)));
    QObject::connect(ui->skelThreshSlider, SIGNAL(valueChanged(int)), ui->skelThreshSpinBox, SLOT(setValue(int)));
    ui->skelThreshSpinBox->setValue(Play::getInstance()->getSkelThreshold());

    QObject::connect(ui->cannyThresh1SpinBox, SIGNAL(valueChanged(int)), ui->canThresh1Slider, SLOT(setValue(int)));
    QObject::connect(ui->canThresh1Slider, SIGNAL(valueChanged(int)), ui->cannyThresh1SpinBox, SLOT(setValue(int)));
    ui->cannyThresh1SpinBox->setValue(Play::getInstance()->getCannyThreshold1());

    QObject::connect(ui->cannyTresh2SpinBox, SIGNAL(valueChanged(int)), ui->canThresh2Slider, SLOT(setValue(int)));
    QObject::connect(ui->canThresh2Slider, SIGNAL(valueChanged(int)), ui->cannyTresh2SpinBox, SLOT(setValue(int)));
    ui->cannyTresh2SpinBox->setValue(Play::getInstance()->getCannyThreshold2());

    QObject::connect(ui->cannyApertureSizeSpinBox, SIGNAL(valueChanged(int)), ui->canApSlider, SLOT(setValue(int)));
    QObject::connect(ui->canApSlider, SIGNAL(valueChanged(int)), ui->cannyApertureSizeSpinBox, SLOT(setValue(int)));
    ui->cannyApertureSizeSpinBox->setValue(Play::getInstance()->getCannyApertureSize());

    // makes the exit button work
    connect(ui->cancelPushButton,SIGNAL(clicked()),this, SLOT(close()));
}

settingsDialog::~settingsDialog()
{
    delete ui;
}


void settingsDialog::on_applyPushButton_clicked()
{
    Play::getInstance()->setMinLength(ui->minLengthDoubleSpinBox->value());
    Play::getInstance()->setMinVote(ui->minVoteSpinBox->value());
    Play::getInstance()->setMaxGap(ui->maxGapDoubleSpinBox->value());
    Play::getInstance()->setSkelThreshold(ui->skelThreshSpinBox->value());
    Play::getInstance()->setCannyThreshold1(ui->cannyThresh1SpinBox->value());
    Play::getInstance()->setCannyThreshold2(ui->cannyTresh2SpinBox->value());
    Play::getInstance()->setCannyApertureSize(ui->cannyApertureSizeSpinBox->value());
    Play::getInstance()->setL2Gradient(ui->l2GradientCheckBox->isChecked());
}

void settingsDialog::on_cancelPushButton_clicked()
{
    // see constructor
}
