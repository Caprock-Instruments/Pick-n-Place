#include "ccontrollers.h"

//REMEMBER; NEED AT LEAST 2 CONTROLLERS FOR 4 HEAD PICK-N-PLACE

//CNTRL-1 (Marlin-GRBL; 5-AXIS)
//X,Y: carriage move
//Z: Nozzles 1,2 (sea-saw)
//E0,E1: Rotation, Nozzles 1,2
//USE Coolant for Flying Camera LED-Lighting

//CNTRL-2 (Marlin-GRBL; 5-AXIS)
//X,Y: avail
//Z: Nozzles 3,4 (sea-saw)
//E0,E1: Rotation, Nozzles 3,4
//USE Coolant for Up-Vision Camera LED-Lighting

//CNTRL-3 (Custom; digital I/O to Auto-Index Auto-Feeders)
// 50 pins avail for digital I/O on Mega


//Note: probably need to think about distributed control so we can minimize wiring;
//use simple daisy-chained serial bus, CAN, or I2C



cControllers::cControllers(QObject *parent) : QObject(parent)
{

}

bool cControllers::run_part_loop()
{
    //get next part type, and place location (from annotated board centroid file)

    //using part_type; get part, using part_indx from cparts

    //using part; check which nozzles we can use; note: how many nozzles we can use

    //using part; get feeder/s (mulitiple if using more than 1 nozzle); note if we use bottom-vision or not

    //using feeder/s; get gcode to retrieve part/s, rotation

    //if using bottom-vision; move nozzle/s to camera and re-align part/s

    //move to place-locations and place part/s
}
