#ifndef CGCODE_H
#define CGCODE_H

#include <QString>
#include <QStringList>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include <math.h>
#include <string.h>


using namespace cv;
using namespace std;

//could take advantage of this being a class and use 1 instantiation for each controller (i.e. 1 per set of 3 axis's)

class cGCode
{
public:
    cGCode();

    QString gcode_dwell_ms(int ms);

    QString gcode_turnOnOff_vacuum(bool on);

    QString gcode_moveX(float end_pt, bool fastMove);
    QString gcode_moveY(float end_pt, bool fastMove);
    QString gcode_moveZ(float end_pt, bool fastMove);

    QString gcode_2d_line(Point3f end_pt, bool fastMove);
    QString gcode_z_line(Point3f end_pt, bool fastMove);
    QString gcode_z_line(Point3f start_pt, Point3f end_pt, float percent_fast_firstPart_move);

    QString gcode_line(Point3f next_pt, bool fastMove);

    QString gcode_relative_2d_line(Point3f end_pt, bool fastMove);

    //assumes we're at starting point of arc;
    QString gcode_arc(Point3f start_pt, Point3f end_pt, Point3f center_pt, bool bFull_circle=false);

    bool gcode_path(vector<Point3f> pts, QStringList &gLines, bool fastMove);

    QStringList gcode_pocket_outline(vector<Point3f> pts, Point3f finish_ht, float depth_cut, float final_depth);

    QString get_gcode_init();

    bool update_rxd_buffer(QString sBuffer);

    void parse_rxd_buffer();

    void clear_buffer();

public:
    Point3f cur_position;  //maintains track of our current position

    QString sController_Buffer;

    bool cmd_run_status = false;

private:




};

#endif // CGCODE_H
