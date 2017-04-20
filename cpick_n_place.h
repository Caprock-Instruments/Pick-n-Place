#ifndef CPICK_N_PLACE_H
#define CPICK_N_PLACE_H

#include <QObject>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include <QString>
#include <QStringList>

#include "cpart.h"
#include "cboard.h"
#include "cnozzles.h"
#include "cgcode.h"
#include "cutils_math.h"


using namespace cv;
using namespace std;

class cPick_n_Place : public QObject
{
    Q_OBJECT
public:
    explicit cPick_n_Place(QObject *parent = 0);

signals:

public slots:

public:
    QStringList gen_gcode_rotation_operation(int axis_to_use, float rotation_degrees);
    QStringList gen_gcode_pick_operation(Point3f nozzle_offset, Point3f feeder_location, int dwell_ms, float lift_off_distance);
    QStringList gen_gcode_place_operation(Point3f nozzle_offset, Point3f feeder_location, int dwell_ms, float lift_off_distance);

public:
    //note: if we add any static variables here to keep track of things; have to change mainwindow code to use
    //a persistant instantiation of this object; currently it's only creating it to gain access to gcode generation functions

    float assumed_cnc_radius = 20.0;  //used to convert from rotary motion to linear mm motion GRBL is expecting


private:


};

#endif // CPICK_N_PLACE_H
