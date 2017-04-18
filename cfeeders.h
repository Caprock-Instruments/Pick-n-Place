#ifndef CFEEDERS_H
#define CFEEDERS_H

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

class cFeeders : public QObject
{
    Q_OBJECT
public:
    explicit cFeeders(QObject *parent = 0);

signals:

public slots:

public:
    Point3f start_feed_location;  //used for strip/tray feeders
    Point3f feed_location; //point where we need to go to get parts; includes z-height for part

    double rotation_angle;  //rotation angle of part

    double dx=0.0;  //offset for trays to next part
    double dy=0.0;
    double dz_dx=0.0;
    double dz_dy=0.0;

    Point tray_indx;  //current index of where next part is in strip/tray; indx y (before x)

    bool is_1d_tray;
    bool is_2d_tray;

    QString sPart;  //part name like pic32
    QString sPackage; //package name


private:


};

#endif // CFEEDERS_H
