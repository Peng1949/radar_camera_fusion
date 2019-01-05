/*************************************************************************
    > Author: z.h
    > Date: 2018.12.17 
************************************************************************/
#include "radar_camera_fusion.h"

Radar_camera_fusion::Radar_camera_fusion()
{

}

Radar_camera_fusion::~Radar_camera_fusion()
{

}

void Radar_camera_fusion::init()
{
    ROS_INFO("begin to run!");
}

cv::Point2f Radar_camera_fusion::transformPoint(const cv::Matx33f &M,
                                           const cv::Point2f &p)
{
    cv::Vec3f v(p.x, p.y, 1);
    cv::Vec3f mv = M * v;

    cv::Point2f res;
    res.x = mv[0] / mv[2];
    res.y = mv[1] / mv[2];

    return res;
}

void Radar_camera_fusion::radar_objects_Callback(const conti_radar_msgs::conti_Objects::ConstPtr &msgs)
{
    cv::Mat img;
    {
        std::lock_guard<std::mutex> lk(mut);
        img = raw_image.clone();
    }
    for(size_t i = 0; i < msgs->objects.size(); ++i)
    {
        cv::Point2f radar_point;

        radar_point.x = -(msgs->objects[i].Object_DistLat);
        radar_point.y = msgs->objects[i].Object_DistLong;

        cv::Point2f image_point = transformPoint(Homograph,radar_point);

        cv::circle(img, cv::Point((int)image_point.x,(int)image_point.y),10,cv::Scalar( 0, 0, 255),-1,8);

        char text[100];
        std::sprintf(text, "%d(%4.2f,%4.2f)", msgs->objects[i].ID,msgs->objects[i].Object_DistLong,-(msgs->objects[i].Object_DistLat));
        cv::putText(img, text, cv::Point((int)image_point.x+5,(int)image_point.y), cv::FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(255,0,0), 2, 8);
    }
    
    cv::imshow("Result", img);
    if ('q' == cv::waitKey(1))
    {
        ros::shutdown();
    }
}


void Radar_camera_fusion::camera_objects_Callback(const sensor_msgs::ImageConstPtr &msgs)
{

    std::lock_guard<std::mutex> lk(mut);
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msgs, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    raw_image = cv_ptr->image.clone();
}


void Radar_camera_fusion::compressed_camera_objects_Callback(const sensor_msgs::CompressedImageConstPtr &msgs)
{
    std::lock_guard<std::mutex> lk(mut);
    
    raw_image = cv::imdecode(cv::Mat(msgs->data), cv::IMREAD_UNCHANGED);
    cv::cvtColor(raw_image, raw_image, cv::COLOR_BayerBG2BGR);
}

void Radar_camera_fusion::doListening()
{    
    std::string camera_topic = "/pylon_camera_1/pylon_camera_node/image_raw/compressed";
    auto pos = camera_topic.find("compressed");
    if (pos != std::string::npos)
    {
        recv_camera_objects_sub = nh.subscribe(camera_topic, 1, &Radar_camera_fusion::compressed_camera_objects_Callback, this);
    }
    else    
    {
        recv_camera_objects_sub = nh.subscribe(camera_topic, 1, &Radar_camera_fusion::camera_objects_Callback, this);
    }

    recv_radar_objects_sub = nh.subscribe("/conti_radar_1/objects", 1, &Radar_camera_fusion::radar_objects_Callback, this);//conti_radar_1/objects
    ros::spin();

}

