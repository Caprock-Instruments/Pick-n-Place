#include "cboard.h"

cBoard::cBoard()
{

}

void cBoard::sort_parts_by_nozzle_size()
{
    //this requires a bool comparison operator be built into class cPart
    std::sort(part_list.begin(), part_list.end());
}
