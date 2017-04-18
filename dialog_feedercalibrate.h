#ifndef DIALOG_FEEDERCALIBRATE_H
#define DIALOG_FEEDERCALIBRATE_H

#include <QDialog>

namespace Ui {
class Dialog_feederCalibrate;
}

class Dialog_feederCalibrate : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_feederCalibrate(QWidget *parent = 0);
    ~Dialog_feederCalibrate();

private:
    Ui::Dialog_feederCalibrate *ui;
};

#endif // DIALOG_FEEDERCALIBRATE_H
