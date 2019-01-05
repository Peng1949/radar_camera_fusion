#!/usr/bin/python

import cv2
import numpy as np
from sensor_msgs.msg import CompressedImage

import rospy

iter_num = 0

def gps_callback(gps):

    if mode == 's':
        f.write('%1.10f %1.10f %1.10f %1.10f %1.10f %1.10f\n'%\
                (gps.pose.pose.position.x, \
                 gps.pose.pose.position.y, \
                 gps.pose.pose.position.z, \
                 gps.pose.pose.orientation.x, \
                 gps.pose.pose.orientation.y, \
                 gps.pose.pose.orientation.z))
    elif mode == 'r':
        print '%1.10f, %1.10f' % (gps.pose.pose.position.x, gps.pose.pose.position.y)
        # plt.plot(gps.pose.pose.position.x, gps.pose.pose.position.y, '.')
        plt.plot(gps.pose.pose.position.x, gps.pose.pose.position.y, 'r.')
        plt.axis('equal')
        plt.pause(0.001)

def image_callback(image):
    np_data = np.fromstring(image.data, np.uint8)
    im = cv2.imdecode(np_data, 0)
    global iter_num

    if iter_num == 10:
        im2 = cv2.cvtColor(im, cv2.COLOR_BAYER_BG2BGR)
    # im2 = cv2.cvtColor(im, cv2.COLOR_BAYER_GB2BGR)

        im2 = cv2.resize(im2, (960, 640))
        save_name = '/home/xd/Data/tianjin/0111_new_night/src/%d.jpg' % (image.header.stamp.to_nsec() / 1000)
        cv2.imwrite(save_name, im2)
        iter_num = 0
        #cv2.imshow('sb', im2)

    iter_num += 1
    cv2.waitKey(1)
    print 'stamp: %d, %d\nformat: %s, len: %d'%(image.header.stamp.to_sec(), \
                                                image.header.stamp.to_nsec(), \
                                                image.format, len(image.data))

if __name__ == '__main__':
    rospy.init_node('save_image', anonymous=True)
    
    rospy.Subscriber('/pylon_camera_node/image_raw/compressed', CompressedImage, image_callback)
    rospy.spin()
