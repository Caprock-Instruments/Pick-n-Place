#ifndef CCONTROLLERS_H
#define CCONTROLLERS_H

#include <QObject>

#include <QString>
#include <QStringList>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include <math.h>
#include <string.h>


using namespace cv;
using namespace std;

class cControllers : public QObject
{
    Q_OBJECT
public:
    explicit cControllers(QObject *parent = 0);

    bool run_part_loop();

signals:

public slots:
};

#endif // CCONTROLLERS_H
