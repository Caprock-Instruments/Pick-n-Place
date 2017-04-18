#ifndef CBOARD_H
#define CBOARD_H

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


using namespace cv;
using namespace std;

//TO CREATE BOARD FILE: in Eagle; run ULP:

class cBoard
{

public:
    cBoard();

    void sort_parts_by_nozzle_size();

signals:

public slots:

public:
    int board_id;
    float board_rotation;
    Point2f board_location;

    vector<Point2f> relative_fiducial_locations;

    //abs fudicial location to use for computing placements
    vector<Point2f> abs_fudicial_locations;  //location after calibration (vision-capture)

    vector<cPart> part_list;   //may need this for upVision ...

    //not sure we need to store this
    QString board_fileName;


private:


};

#endif // CBOARD_H
