/*************************************************************************
    > Author: z.h
    > Date: 2018.12.17 
************************************************************************/

#include "radar_camera_fusion.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "radar_camera_fusion");

    Radar_camera_fusion *radar_camera_fusion = new Radar_camera_fusion;

    radar_camera_fusion->init();
    radar_camera_fusion->doListening();


    delete(radar_camera_fusion);

    return 0;
}
