#!/usr/bin/env python
#coding=utf-8

import urx
import time
import rospy

class imp_control:
    def __init__(self,node):
        self.Node=node
        self.M_param=(1.6,1.6,3.2,0.4,0.3,0.2)
        self.B_param=(4.8,4.8,3.6,1.6,1.2,0.6)
        self.forceSensor(0,0,1,0,0,0)
        self.robCurPose=(0,0,0,0,0,0)
        self.robNextPose=(0,0,0,0,0,0)






if __name__ == '__main__':
    nodeName = "py_imp_control"
    rospy.init_node(nodeName)
    rospy.spin()