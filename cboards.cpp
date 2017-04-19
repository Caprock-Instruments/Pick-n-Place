#include "cboards.h"

cBoards::cBoards()
{

}

void cBoards::sort_parts_by_nozzle_size()
{
    //this requires a bool comparison operator be built into class cPart
    std::sort(master_parts_list.begin(), master_parts_list.end());
}

void cBoards::append_part_to_master_list(cBoard board)
{

    master_parts_list.insert(master_parts_list.end(), board.part_list.begin(), board.part_list.end());
}

void cBoards::clear_master_list()
{

}
