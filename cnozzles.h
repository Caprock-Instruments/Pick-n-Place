#ifndef CNOZZLES_H
#define CNOZZLES_H

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

#include "cnozzle.h"


using namespace cv;
using namespace std;

class cNozzles
{
public:
    cNozzles();

signals:

public slots:

public:
    bool insert_nozzle_into_list(cNozzle nozzle);


    vector<cNozzle> nozzle_list;

    //position relative to camera (which is our cal-point), z-height is physical distance off base-height of starting point
    vector<Point3f> nozzle_positions_list;


private:

};

#endif // CNOZZLES_H
