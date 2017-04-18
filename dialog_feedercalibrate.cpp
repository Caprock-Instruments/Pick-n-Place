#include "dialog_feedercalibrate.h"
#include "ui_dialog_feedercalibrate.h"

Dialog_feederCalibrate::Dialog_feederCalibrate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_feederCalibrate)
{
    ui->setupUi(this);
}

Dialog_feederCalibrate::~Dialog_feederCalibrate()
{
    delete ui;
}
