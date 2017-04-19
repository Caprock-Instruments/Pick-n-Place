#include "cboard.h"

cBoard::cBoard()
{

}

void cBoard::sort_parts_by_nozzle_size()
{
    //this requires a bool comparison operator be built into class cPart
    std::sort(part_list.begin(), part_list.end());
}

bool cBoard::compute_corrections()
{
    bool ok = true;

    int P = static_cast<int>(absolute_fudicial_locations.size());

    if(P<2)
        return false; //flag error, haven't measured fiducials yet


    compute_absolute_board_rotation();

    ok = compute_XY_corrections();

    return ok;
}

//based on measured (absolute) cnc positions; corrects for rotation (assumes already computed)
//also assumes we have cnc-vision measured absolute_fiducial_locations
bool cBoard::compute_XY_corrections()
{
    cUtils_Math utils_math;

    Point2f theory_pt1 = relative_fiducial_locations[0];
    Point2f theory_pt2 = relative_fiducial_locations[1];
    Point2f meas_pt1 = absolute_fudicial_locations[0];
    Point2f meas_pt2 = absolute_fudicial_locations[1];

    Point2f dydx;

    bool ok = utils_math.gain_dydx_between_2_points(board_rotation, theory_pt1, theory_pt1, meas_pt1, meas_pt2, dydx);

    dx_correction = dydx.x;
    dy_correction = dydx.y;

    return ok;
}


//computes absolute part centroid for all our parts
bool cBoard::compute_absolute_part_centroid()
{
    bool ok = false;

    int N = static_cast<int>(part_list.size());

    Point2f first_absolute_fiducial_location = absolute_fudicial_locations[0];  //center of rotation

    for(int i=0;i<N;i++)
    {
        cPart part = part_list[i];

        ok = part.compute_absolute_part_centroid(board_rotation, first_absolute_fiducial_location);

    }

    return ok;
}


void cBoard::compute_absolute_board_rotation()
{
    cUtils_Math utils_math;
    double fiduc_angle_degrees, board_angle_degs;

    Point2f pt1 = relative_fiducial_locations[0];
    Point2f pt2 = relative_fiducial_locations[1];

    utils_math.angle_between_2_points(pt1, pt2, fiduc_angle_degrees);

    //assumes fudicial 1 (lower left on board) is measured before fudicial2!!! because we're counting on order being correct!
    Point2f bd_pt1 = absolute_fudicial_locations[0];
    Point2f bd_pt2 = absolute_fudicial_locations[1];

    utils_math.angle_between_2_points(bd_pt1, bd_pt2, board_angle_degs);

    board_rotation = board_angle_degs - fiduc_angle_degrees;

}
