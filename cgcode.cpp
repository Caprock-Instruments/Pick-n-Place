#include "cgcode.h"

cGCode::cGCode()
{

}

QString cGCode::gcode_arc(Point3f start_pt, Point3f end_pt, Point3f center_pt, bool bFull_circle)
{
    QString sGcode_line;

    float i = end_pt.x - start_pt.x;
    float j = end_pt.y - start_pt.y;

    QString sI = "I" + QString::number(i,'f',4);
    QString sJ = "J" + QString::number(j,'f',4);

    float x = end_pt.x;
    float y = end_pt.y;

    QString sX = "X" + QString::number(x,'f',4);
    QString sY = "Y" + QString::number(y,'f',4);

    if(bFull_circle==false)
    {
        sGcode_line = sX + " " + sY + " " + sI + " " + sJ + ";\n";
    }
    else
    {
        sGcode_line =  sI + " " + sJ + ";\n";
    }


    return sGcode_line;
}


QString cGCode::gcode_relative_2d_line(Point3f end_pt, bool fastMove)
{
    Point3f abs_end_pt;

    abs_end_pt.z = cur_position.z;

    abs_end_pt.x = cur_position.x + end_pt.x;
    abs_end_pt.y = cur_position.y + end_pt.y;


    QString gcodeLine = gcode_2d_line(abs_end_pt, fastMove);

    return gcodeLine;
}

QString cGCode::gcode_2d_line(Point3f next_pt, bool fastMove)
{
    QString gLine;

    if(fastMove)
        gLine = "G01 ";
    else
        gLine = "G00 ";

    float x, y;

    x = next_pt.x;
    y = next_pt.y;

    QString sX, sY;

    sX = QString::number(x,'f',4);
    sY = QString::number(y,'f',4);

    gLine += "X" + sX + " Y" + sY + ";\n";

    return gLine;
}

QString cGCode::gcode_line(Point3f next_pt, bool fastMove)
{
    QString gLine;

    if(fastMove)
        gLine = "G01 ";
    else
        gLine = "G00 ";

    float x, y, z;

    x = next_pt.x;
    y = next_pt.y;
    z = next_pt.z;

    QString sX, sY, sZ;

    sX = QString::number(x,'f',4);
    sY = QString::number(y,'f',4);
    sZ = QString::number(z,'f',4);

    gLine += "X" + sX + " Y" + sY + " Z" + sZ + "\n";

    return gLine;
}

bool cGCode::gcode_path(vector<Point3f> pts, QStringList &gLines, bool fastMove)
{
    bool ok = false;
    QString gLine;

    int N = static_cast<int>(pts.size());


    for(int i=0; i<N; i++)
    {
        Point3f pt = pts[i];
        gLine = gcode_line(pt, fastMove);

        int len = gLine.length();

        if(len > 0)
        {
            ok = true;
            gLines.append(gLine);
        }
    }

    return ok;
}

//mills outline of pocket; assumes points already compensate for cutter dia.
//assumes we're at traveling ht
//assumes we're at x,y starting point of cut
//assumes top of part is z=0.0
QStringList cGCode::gcode_pocket_outline(vector<Point3f> pts, Point3f finish_ht, float depth_cut, float final_depth)
{
    QStringList gLines;
    QStringList master_gLines;
    bool ok;

    bool fastMove = false;

    float dz = fabs(depth_cut);
    float cur_depth = 0.0;

    int N = static_cast<int>(pts.size());


    if(N <= 0)
    {
        //return error
        return gLines;
    }


    while(cur_depth > final_depth)
    {
        //calc next depth
        cur_depth -= dz;

        if(cur_depth < final_depth)
        {
            cur_depth = final_depth;
        }

        //drop to cut depth
        Point3f next_pt;
        next_pt.x = pts[0].x;
        next_pt.y = pts[0].y;
        next_pt.x = -dz;


        QString gLine = gcode_2d_line(next_pt, fastMove);
        gLines.append(gLine);

        cur_depth = -dz;

        //make cut
        //ok = gcode_path(pts, gLines, fastMove);
    }


    return gLines;
}


QString cGCode::get_gcode_init()
{
    QString gline = "G10 P0 L20 X0 Y0 Z0\n;";

    return gline;
}

bool cGCode::update_rxd_buffer(QString sBuffer)
{
    sController_Buffer += sBuffer;

    parse_rxd_buffer();
}

void cGCode::parse_rxd_buffer()
{
//1) SEND GCODE
//2) WAIT FOR 'OK' or 'error:##'
//3) At this point it will be in RUN mode and a "?" will return
    //<Run|MPos:7.420,0.000,0.000|FS:500,0|WCO:0.000,0.000,0.000>   //note: at 115200 baud; update rate is faster than 7ms
//4) Keep polling until you get an Idle In message

    //<Idle|MPos:0.000,25.400,0.000|FS:0,0|Ov:100,100,100>
//5) Extract final position & update our position
//6) set done flag


}

void cGCode::clear_buffer()
{
    sController_Buffer = "";
}
