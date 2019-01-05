#!/usr/bin/env python
# -*-coding: utf-8 -*-

""" classify images to different dirs.
"""

import os
import argparse
import cv2
import rosbag
from sensor_msgs.msg import Image
from cv_bridge import CvBridge

def main():
    parser = argparse.ArgumentParser(description="classify images to different dirs.")
    parser.add_argument("images_dir", help="Input images.")
    parser.add_argument("output_dir", help="Output directory.")
   
    args = parser.parse_args()

    print "classify images from %s into %s" % (args.images_dir, args.output_dir)
 
    for root, dirs, files in os.walk(args.images_dir):  
	for file in files:  
		if os.path.splitext(file)[1] == '.jpg':  
			img_file = os.path.join(root, file)
			print img_file 
			img = cv2.imread(img_file)
                        cv2.imshow("img", img)
			k = cv2.waitKey(0)
			if k == ord('j'):
				print "save image"
   				cv2.imwrite(os.path.join(args.output_dir, file), img)
    return

if __name__ == '__main__':
    main()

