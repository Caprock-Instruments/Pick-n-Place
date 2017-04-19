#ifndef CUTILS_MATH_H
#define CUTILS_MATH_H

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

using namespace cv;
using namespace std;

#define PI M_PI

class cUtils_Math
{
public:
    cUtils_Math();

    const double eps = 0.0001;


    bool angle_between_2_points(Point2f pt1, Point2f pt2, double &angle_degrees);
    bool distance_between_2_points(Point2f pt1, Point2f pt2, double &distance);

    bool rotate_point2_about_point1(double rotation_degrees, Point2f pt1, Point2f pt2, Point2f &rotated_pt);

    bool gain_dydx_between_2_points(double rotation, Point2f theory_pt1, Point2f theory_pt2, Point2f meas_pt1, Point2f meas_pt2, Point2f &dydx);

};

#endif // CUTILS_MATH_H
