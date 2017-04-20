#include "cutils_math.h"

cUtils_Math::cUtils_Math()
{

}

//map degrees rotation to linear distance
bool cUtils_Math::radial_distance(double radius, double degrees_rotation, double &radial_distance)
{
    radial_distance = 2.0* PI * radius * degrees_rotation/360.0;

    return true;
}


bool cUtils_Math::distance_between_2_points(Point2f pt1, Point2f pt2, double &distance)
{
    double dx = pt2.x - pt1.x;
    double dy = pt2.y - pt1.y;

    double dist = std::sqrt(dx*dx + dy*dy);

    if(dist < eps)
    {
        return false;
    }

    return true;
}

//computes gain in x and y between 2 points;
bool cUtils_Math::gain_dydx_between_2_points(double rotation, Point2f theory_pt1, Point2f theory_pt2, Point2f meas_pt1, Point2f meas_pt2, Point2f &dydx)
{
    bool ok = true;

    cUtils_Math utils_math;
    double fiduc_angle_degrees, distance, dx, dy;

    Point2f rel_pt1 = theory_pt1;  //bd layout fiducial points
    Point2f rel_pt2 = theory_pt1;

    Point2f abs_pt1 = meas_pt1;  //cnc measured fiducial points
    Point2f abs_pt2 = meas_pt2;  //cnc measured fiducial points


    ok = distance_between_2_points(rel_pt1,rel_pt2, distance);

    if(!ok)
        return false;

    dx = rel_pt2.x - rel_pt1.x;
    dy = rel_pt2.y - rel_pt1.y;

    if((dx < eps)||(dy<eps))
        return false;

    dx = abs_pt2.x - abs_pt1.x;
    dy = abs_pt2.y - abs_pt1.y;

    if((dx < eps)||(dy<eps))
        return false;


    utils_math.angle_between_2_points(rel_pt1, rel_pt2, fiduc_angle_degrees);

    Point2f rel_pt2_rot_corrected;  //bd layout fiducial point after rotation by how bd is placed into system
    rotate_point2_about_point1(rotation, rel_pt1, rel_pt2, rel_pt2_rot_corrected);

    double bd_dx, bd_dy, meas_dx, meas_dy;  //dydx as spec'd in bd layout (Gerber file)

    bd_dx = rel_pt2_rot_corrected.x - rel_pt1.x;
    bd_dy = rel_pt2_rot_corrected.y - rel_pt1.y;

    //now get cnc measured dy dx
    meas_dx = abs_pt2.x - abs_pt1.x;
    meas_dy = abs_pt2.y - abs_pt1.y;

    double dx_correction = bd_dx / meas_dx;
    double dy_correction = bd_dy / meas_dy;

    dydx.x = (float)dx_correction;
    dydx.y = (float)dy_correction;


    return ok;
}

//computes angle in degrees between 2 points
bool cUtils_Math::angle_between_2_points(Point2f pt1, Point2f pt2, double &angle_degrees)
{
    bool ok = false;

    double x1 = pt1.x;
    double y1 = pt1.y;
    double x2 = pt2.x;
    double y2 = pt2.y;

    double dy = y2 - y1;
    double dx = x2 - x1;



    double angle_rads = atan2(dy,dx);

    angle_degrees = 360.0*angle_rads/(2.0*PI);

    ok = true;

    return ok;
}

bool cUtils_Math::rotate_point2_about_point1(double rotation_degrees, Point2f pt1, Point2f pt2, Point2f &rotated_pt)
{
    //need to computed postion of rotated part
    double x1 = pt1.x;
    double y1 = pt1.y;
    double x2 = pt2.x;
    double y2 = pt2.y;

    double dx = x2 - x1;
    double dy = y2 - y1;

    //compute radius and angle
    double radius = std::sqrt(dx*dx + dy*dy);

    double angle_degrees, angle_radians, rotation_angle_radians;

    angle_degrees = angle_between_2_points(pt1, pt2, angle_degrees);

    angle_radians = 2.0*PI*angle_degrees/360.0;
    rotation_angle_radians = 2.0*PI*rotation_degrees/360.0;

    double new_angle_radians = angle_radians + rotation_angle_radians;

    double new_x = radius * cos(new_angle_radians);
    double new_y = radius * sin(new_angle_radians);

    rotated_pt.x = new_x;
    rotated_pt.y = new_y;

    return true;
}
