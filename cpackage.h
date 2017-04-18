#ifndef CPACKAGE_H
#define CPACKAGE_H

#include <QObject>

//One instance of a package

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
#include "cnozzles.h"

using namespace cv;
using namespace std;

class cPackage
{
public:
    cPackage();

    bool load_compatible_nozzle_list();

signals:

public slots:

public:

    QString sPackageName; //SOIC8
    int num_pins;

    float package_height;  //needed mostly from tube/tray as tapes have part recesses so top of part is at top of tape

    Rect2f package_body_size;
    Rect2f package_total_size;

    Mat template_image;

    vector<cNozzle> compatible_nozzle_list;


private:


};

#endif // CPACKAGE_H
