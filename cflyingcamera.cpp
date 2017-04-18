#include "cflyingcamera.h"

cFlyingCamera::cFlyingCamera(QObject *parent) : QObject(parent)
{
    fCameraOffset_mm.x = 0.0;
    fCameraOffset_mm.y = 0.0;

    cgc = new cGCode();  //only use access to gcode generation routines; not to keep track of position!
}


Point2f cFlyingCamera::convert_pixelOffset_to_mm(Point2f pixel_offset)
{
    Point2f mm_offset;
    float x_mm, y_mm;

    x_mm = pixel_offset.x * pixel_to_mm_gain.x;
    y_mm = pixel_offset.y * pixel_to_mm_gain.y;

    mm_offset.x = x_mm;
    mm_offset.y = y_mm;

    return mm_offset;
}

Point2f cFlyingCamera::convert_mm_to_pixelOffset(Point2f mm_offset)
{
    Point2f pixel_offset;
    float mm_to_pixel_gain_x, mm_to_pixel_gain_y;

    pixel_offset.x = -999999.0;
    pixel_offset.y = -999999.0;

    if((pixel_to_mm_gain.x != 0.0)&&(pixel_to_mm_gain.y!=0.0))
    {
        mm_to_pixel_gain_x = 1.0 / pixel_to_mm_gain.x;
        mm_to_pixel_gain_y = 1.0 / pixel_to_mm_gain.y;
    }
    else
    {
        return pixel_offset;
    }

    pixel_offset.x = mm_offset.x * mm_to_pixel_gain_x;
    pixel_offset.y = mm_offset.y * mm_to_pixel_gain_y;

    return pixel_offset;
}

QString cFlyingCamera::get_gcode_moveTo(Point2f pixel_offset, bool fastMove)
{
    //compute end_pt
    Point3f end_pt;

    end_pt.z = 0.0;

    Point2f end_pt_offset = convert_pixelOffset_to_mm(pixel_offset);

    end_pt.x = end_pt_offset.x;
    end_pt.y = end_pt_offset.y;

    QString gcodeLine = cgc->gcode_relative_2d_line(end_pt, fastMove);

    return gcodeLine;
}


