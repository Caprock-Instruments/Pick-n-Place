#include "cfeeder.h"

cFeeder::cFeeder()
{

}

QStringList cFeeder::gen_gcode_pick_operation(Point2f cur_position, int nozzle_indx)
{

}

//this will auto-index automatic feeders or will increment tray location to next cell
bool cFeeder::index_to_next_location(QStringList &gCode_lines)
{

}

//get feeder configure from user via gui
bool cFeeder::configure_feeder()
{

}

//load feeder configuration from file
bool cFeeder::load_feeder(QString filePathName)
{

}
