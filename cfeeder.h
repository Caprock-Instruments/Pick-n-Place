#ifndef CFEEDER_H
#define CFEEDER_H

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

#include "cpackage.h"
#include "cnozzle.h"
#include "cnozzles.h"


using namespace cv;
using namespace std;

//width of SOIC 18-28 Tube: 14.70mm


class cFeeder
{

public:
    cFeeder();

    //returns gcode that moves to this feeder and performs pick operation
    QStringList gen_gcode_pick_operation(Point2f cur_position, int nozzle_indx);

signals:

public slots:

public:

    Point3f location;
    int feeder_id;
    int location_id;
    int num_parts_left;

    bool index_to_next_location(QStringList &gCode_lines);

    //get user input to setup feeder,
    bool configure_feeder();

    //load feeder configuration from file
    bool load_feeder(QString filePathName);

    cPackage package;  //what package this feeder is holding


    Point3f start_feed_location;  //used for strip/tray feeders
    Point3f feed_location; //point where we need to go to get parts; includes z-height for part

    double rotation_angle;  //rotation angle of part

    double dx=0.0;  //offset for trays to next part
    double dy=0.0;
    double dz_dx=0.0;
    double dz_dy=0.0;

    Point tray_indx;  //current index of where next part is in strip/tray; indx y (before x)

    bool is_1d_tray=true;
    bool is_2d_tray=false;

    QString sPartType;  //part name like {CAP, RES, INDUCT, AUML, IC, VREG}
    QString sPartValue; //part value; like {0.01uF, 10k, AUML, PIC32, ...}
    QString sPackage; //package name like {C0805, C1205, R0805, ... ,SO-08, SOIC8}
    QString sFeederType;  //type of feeder {STRIP, TUBE, TRAY, AUTOMATIC}

private:


};

#endif // CFEEDER_H
