#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright 2016 Massachusetts Institute of Technology

"""Extract images from a rosbag.
"""

import os
import argparse

import cv2
import numpy as np

import rosbag
from sensor_msgs.msg import Image
from cv_bridge import CvBridge

def main():
    """Extract a folder of images from a rosbag.
    """
    parser = argparse.ArgumentParser(description="Extract images from a ROS bag.")
    parser.add_argument("bag_file", help="Input ROS bag file.")
    parser.add_argument("output_dir", help="Output directory.")
    parser.add_argument("image_topic", help="Image topic.")

    args = parser.parse_args()

    print "Extract images from %s on topic %s into %s" % (args.bag_file,
                                                          args.image_topic, args.output_dir)

    bridge = CvBridge()
    num = 0
    count = 0

    bag = rosbag.Bag(args.bag_file, "r")
    for topic, msg, t in bag.read_messages(topics=[args.image_topic]):
        if count%10 == 0:
            img_data = np.fromstring(msg.data, np.uint8)
            cv_img = cv2.imdecode(img_data, cv2.IMREAD_COLOR)
		
            #cv_img = cv2.cvtColor(cv_img, cv2.COLOR_BayerGB2BGR) #COLOR_BayerBG2BGR
            timestr = "%d" % (msg.header.stamp.to_nsec() / 1000)
            cv2.imwrite(os.path.join(args.output_dir, timestr + '.jpg'), cv_img)
            print "Wrote image %i" % num
            num += 1
        count += 1
    bag.close()
    return

if __name__ == '__main__':
    main()
