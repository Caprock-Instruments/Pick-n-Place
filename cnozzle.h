#ifndef CNOZZLE_H
#define CNOZZLE_H

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


class cNozzle
{
public:
    cNozzle();

signals:

public slots:

public:

    //nozzle_loc_id, nozzle_type, z_height, n_packages_it_can_place
    //1, N503, -1.10, C0603, R0603, L0603, C0805, R0805, L0805

    int nozzle_loc_id;
    QString sNozzleType;

    QString sNozzle;  //currently loaded nozzle :{N502, N503, N504, ... , N510}
    QStringList sPackages; //list of packages this nozzle can place

    double zHeight;  //distance down from zero-line (bottom face of nozzle-holder); nozzle pick-up point is located


private:



};

#endif // CNOZZLE_H
