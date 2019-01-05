/*************************************************************************
    > Author: z.h
    > Date: 2018.12.17 
************************************************************************/
#ifndef RADAR_CAMERA_FUSION_H
#define RADAR_CAMERA_FUSION_H

#include "ros/ros.h"
#include "conti_radar_msgs/conti_Objects.h"
#include <sensor_msgs/CompressedImage.h>
#include <sensor_msgs/Image.h>

#include <cv_bridge/cv_bridge.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <stdio.h>
#include <string.h>


#include <mutex>

const cv::Matx33f Homograph(
	            1, 1, 1,
	            1, 1, 1,
	            1, 1, 1);

class Radar_camera_fusion
{
private:
	ros::NodeHandle nh;
	ros::Subscriber recv_radar_objects_sub;
	ros::Subscriber recv_camera_objects_sub;
	mutable std::mutex mut;
	cv::Mat raw_image;



public:
	Radar_camera_fusion();
	~Radar_camera_fusion();

	void init();
	static cv::Point2f transformPoint(const cv::Matx33f &M,const cv::Point2f &p);
	void radar_objects_Callback(const conti_radar_msgs::conti_Objects::ConstPtr &msgs); 
	void camera_objects_Callback(const sensor_msgs::ImageConstPtr &msgs);
	void compressed_camera_objects_Callback(const sensor_msgs::CompressedImageConstPtr &msgs);
	void doListening();
};



#endif