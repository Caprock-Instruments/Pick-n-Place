#ifndef CFLYINGCAMERA_H
#define CFLYINGCAMERA_H

#include <QObject>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>

#include "cutils.h"
#include "cgcode.h"


using namespace cv;
using namespace std;


class cFlyingCamera : public QObject
{
    Q_OBJECT
public:
    explicit cFlyingCamera(QObject *parent = 0);

    Point2f convert_pixelOffset_to_mm(Point2f pixel_offset);
    Point2f convert_mm_to_pixelOffset(Point2f mm_offset);

    QString get_gcode_moveTo(Point2f pixel_offset, bool fastMove);

signals:

public slots:


public:
    cGCode *cgc;

    Point3f fCameraOffset_mm;  //or do we make the camera center of the head?  sounds good actually

    Point2f pixel_to_mm_gain;  //gain transform


private:



};

#endif // CFLYINGCAMERA_H
