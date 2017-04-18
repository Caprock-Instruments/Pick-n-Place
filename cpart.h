#ifndef CPART_H
#define CPART_H

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

using namespace cv;
using namespace std;


class cPart
{

public:
    cPart();

    //add boolean ops so we can run QSort; http://www.java2s.com/Code/Cpp/Qt/QuicksortQStringList.htm
    //better alternative (more type-safe); use std::sort  http://stackoverflow.com/questions/4925665/using-qsort-with-class-pointers
    // http://stackoverflow.com/questions/2758080/how-to-sort-stl-vector
    //sort by nozzle size

    //less_than operator for this class; req'd if using std::sort(cpart.begin(), cpart.end()) operation
    bool operator<(cPart &rhs) const;

    //comparison function for 'bool operator<()'
    bool compare_for_sort(QString lhs_sNozzleType, QString rhs_sNozzleType) const;  //note: must be a const function as well

signals:

public slots:

public:
    //REDUNDANT INFO THAT'S ALSO IN FEEDER INFO... DO WE WANT IT HERE TOO?
    cPackage package;   //C0805, ... SOIC8
    QString sPackage;
    QString part_type;  //IC, RES, INDUCT, CAP, AUML
    QString part_value; //pic32
    QString part_Id;  //C10, R12, ...

    QString sNozzleType;  //N501, ... , N510; used for sorting op


    int feeder_id; //feeder id of what we're placing and where it is and rotation

    int nozzle_id; //a nozzle_id for each part, so we know which nozzle and can get specs on what it'll place
    int nozzle_def_list_to_use; //which set of nozzle lists to use

    Point2f part_centroid;  //place on board to place it
    float part_place_rotation;  //orientation on board to place it

    bool start_fresh_nozzle_fill=false; //when we get to this part; this var tells us to go place what's in nozzle first before loading this 1

    bool use_bottomVision_alignment=false;

private:


};

#endif // CPART_H
