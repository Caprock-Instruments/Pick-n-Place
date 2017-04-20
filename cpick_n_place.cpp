#include "cpick_n_place.h"

cPick_n_Place::cPick_n_Place(QObject *parent) : QObject(parent)
{

}


QStringList cPick_n_Place::gen_gcode_rotation_operation(int axis_to_use, float rotation_degrees)
{
    cUtils_Math cutm;
    QStringList gcode_lines;
    cGCode cgc;
    double radial_distance;
    QString gLine;

    cutm.radial_distance(assumed_cnc_radius, (double) rotation_degrees, radial_distance);

    switch(axis_to_use)
    {
        case 0:
            //axis_Letter = "X";
            gLine = cgc.gcode_moveX(radial_distance,true);
            break;
        case 1:
            gLine = cgc.gcode_moveY(radial_distance,true);
            break;
        case 2:
            gLine = cgc.gcode_moveZ(radial_distance,true);
            break;

    }

    gcode_lines.append(gLine);

    return gcode_lines;
}

QStringList cPick_n_Place::gen_gcode_pick_operation(Point3f nozzle_offset, Point3f feeder_location, int dwell_ms, float lift_off_distance)
{
    QStringList gcode_lines;
    cGCode cgc;

    //compute where camera point over feeder should be so our nozzle is over feeder
    Point3f point_over_feeder, point_above_feeder, point_on_feeder;

    point_over_feeder.x = feeder_location.x - nozzle_offset.x;
    point_over_feeder.y = feeder_location.y - nozzle_offset.y;
    point_over_feeder.z = nozzle_offset.z;

    point_above_feeder = point_over_feeder;
    point_above_feeder.z = feeder_location.z + lift_off_distance;

    point_on_feeder = point_over_feeder;
    point_on_feeder.z = feeder_location.z;

    //move XY so that nozzle is directly over feeder
    //QString gcode_2d_line(Point3f end_pt, bool fastMove);
    QString gcd_line = cgc.gcode_2d_line(point_over_feeder, true);

    gcode_lines.append(gcd_line);

    //move Z fast so nozzle is just above part
    bool fast_move = true;
    cgc.gcode_z_line(point_above_feeder, fast_move);

    gcode_lines.append(gcd_line);


    //move Z so nozzle is contacting part
    fast_move = false;
    cgc.gcode_z_line(point_on_feeder, fast_move);

    gcode_lines.append(gcd_line);


    //dwell
    gcd_line = cgc.gcode_dwell_ms(dwell_ms);

    gcode_lines.append(gcd_line);

    //raise up slowly for part lift-off
    //move Z slowly so nozzle is just above part
    cgc.gcode_z_line(point_above_feeder, fast_move);

    gcode_lines.append(gcd_line);

    //raise to full height fast
    fast_move = true;
    cgc.gcode_z_line(point_above_feeder, fast_move);

    gcode_lines.append(gcd_line);



}
