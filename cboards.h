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
#include "cnozzles.h"


using namespace cv;
using namespace std;

class cBoards
{
public:
    cBoards();

    void sort_parts_by_nozzle_size();

    bool assign_all_parts_to_nozzles(vector<cNozzles> nozzles_defs_list, const int MAX_NUM_NOZZLES);

    void append_part_to_master_list(cBoard board);

    void clear_master_list();

public:
    vector<cPart> master_parts_list;   //need to group all the boards togeather so we can sort them correctly so we're not changing nozzles more than 1x ...

    vector<cBoard> master_board_list;

private:

    void init_nozzle_fill_status(QStringList &nozzle_fill_status, const int MAX_NUM_NOZZLES);
    bool find_compatible_nozzles(cPart part, vector<int> &compatibleNozzles_list, cNozzles nozzle_list);

    bool assign_nozzle(cPart part, QStringList &nozzle_fill_status, vector<cNozzles> nozzles_defs_list, const int MAX_NUM_NOZZLES);


    int nozzle_Id_to_use;
    bool start_fresh_fill;
    int  nozzle_def_list_to_use;

};

#endif // CBOARDS_H
