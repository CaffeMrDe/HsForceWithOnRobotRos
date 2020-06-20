#Filename:ur5_drive.py
#!/usr/bin/env python
#coding=utf-8


import urx
import time
rob = urx.Robot("192.168.100.2")
gl_acc=0.1
gl_vel=0.5


def ur_init():
    rob.set_tcp((0,0,0,0,0,0))
    rob.set_payload(0.5, (0,0,0))

def ur_getpose():
    pose =rob.getl()
    return tuple(pose)

def ur_moveL(p1,p2,p3,p4,p5,p6):
    t1=time.time()
    print "python movel begin "
    print (p1,p2,p3,p4,p5,p6)
    # rob.movel((p1, p2, p3, p4, p5, p6),wait=False, acc=gl_acc, vel=gl_vel)
    rob.servoc((p1, p2, p3, p4, p5, p6),acc=gl_acc,vel=gl_vel,wait=True)
    t2 = time.time() - t1
    print "time: "+str(t2)
    print "python movel end "


def ur_close():
    rob.close()
    print "close robot"




# def ur_init():
#     acc=0.01
#     vel=0.1
#     print "python init "
#
#
# def ur_getpose():
#     # return tuple(p1,p2,p3,p4,p5,p6)
#     return (11,22,33,44,55,66)
#
# def ur_moveL(p1,p2,p3,p4,p5,p6):
#     print "python ur_moveL "
#     # self.rob.movel(pose, acc=self.acc, vel=self.vel)
#
# def ur_close():
#     print "close robot"


# class UR5_Move:
#     def __init__(self):
#         self.rob = urx.Robot("192.168.100.2")
#         self.rob.set_tcp((0,0,0,0,0,0))
#         self.rob.set_payload(0.5, (0,0,0))
#         self.acc=0.01
#         self.vel=0.1
#         print "class UR5_Move init over"
#
#     def ur_getpose(self):
#         pose =self.rob.getl()
#         return "{},{},{},{},{},{}".format(*pose)
#
#     def ur_moveL(self,pose):
#         self.rob.movel(pose, acc=self.acc, vel=self.vel)
#
#     def ur_close(self):
#         self.rob.close()
#         print "close robot"



