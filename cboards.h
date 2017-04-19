#ifndef CBOARDS_H
#define CBOARDS_H

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


using namespace cv;
using namespace std;

class cBoards
{
public:
    cBoards();

    void sort_parts_by_nozzle_size();

    void append_part_to_master_list(cBoard board);

    void clear_master_list();

public:
    vector<cPart> master_parts_list;   //need to group all the boards togeather so we can sort them correctly so we're not changing nozzles more than 1x ...



private:


};

#endif // CBOARDS_H
