#!/usr/bin/python2
#Filename:copy_ur5_drive.py


import urx
import time


def ur_getpose():
    # pose =rob.getl()
    print "ur_getpose"
    # return pose

def ur_moveL(pose):
    # rob.movel(pose, acc=self.acc, vel=self.vel)
    pass

def ur_close():
    # rob.close()
    pass

def hellos():
    pass

print "-----work-----"


class UR5_Move:
    def __init__(self,acc,vel):
        # self.rob = urx.Robot("192.168.100.2")
        # self.rob.set_tcp((0,0,0,0,0,0))
        # self.rob.set_payload(0.5, (0,0,0))
        self.acc=acc
        self.vel=vel
        print "init over"

    def ur_getpose(self):
        # pose =self.rob.getl()
        print "ur_getpose"
        # return pose

    def ur_moveL(self,pose):
        pass
        # self.rob.movel(pose, acc=self.acc, vel=self.vel)

    def ur_close(self):
        pass
        # self.rob.close()


# rob = urx.Robot("192.168.1.100") # your robot ip
# rob.set_tcp((0,0,0,0,0,0))       # tool center point
# rob.set_payload(0.5, (0,0,0))
# try:
#     l = 0.05                    # unit: meter
#     v = 0.01
#     a = 0.1
#     pose = rob.getl()           # x,y,z,rx,ry,rz
#     print("robot tcp is at: ", pose)
#     print("absolute move in base coordinate ")
#     pose[2] += l
#     rob.movel(pose, acc=a, vel=v)
#     print("relative move in base coordinate ")
#     rob.translate((0, 0, -l), acc=a, vel=v)
#     print("relative move back and forth in tool coordinate")
#     rob.translate_tool((0, 0, -l), acc=a, vel=v)
#     rob.translate_tool((0, 0, l), acc=a, vel=v)
# finally:
#     rob.close()





# rob = urx.Robot("192.168.100.2")
# rob.set_tcp((0, 0, 0.1, 0, 0, 0))
# rob.set_payload(1, (0, 0, 0.1))
# time.sleep(1)


# orientation = rob.get_orientation()
# pose = rob.get_actual_tcp_pose
# print orientation
# pos = rob.get_pos()
# pose = rob.get_pose()
# print pose.orient.col_x
# print pos["X"], pos["Y"], pos["Z"], pos["Rx"], pos["Ry"], pos["Rz"]
# print pos
# print pose


# print pos.

# sleep(0.2)  #leave some time to robot to process the setup commands
# rob.movej((1, 2, 3, 4, 5, 6), a, v)
# rob.movel((x, y, z, rx, ry, rz), a, v)
# print "Current tool pose is: ",  rob.getl()
# rob.movel((0.1, 0, 0, 0, 0, 0), a, v, relative=true)  # move relative to current pose
# rob.translate((0.1, 0, 0), a, v)  #move tool and keep orientation
# rob.stopj(a)
#
# robot.movel(x, y, z, rx, ry, rz), wait=False)
# while True :
#     sleep(0.1)  #sleep first since the robot may not have processed the command yet
#     if robot.is_program_running():
#         break
#
# robot.movel(x, y, z, rx, ry, rz), wait=False)
# while.robot.getForce() < 50:
#     sleep(0.01)
#     if not robot.is_program_running():
#         break
# robot.stopl()

# try:
#     robot.movel((0,0,0.1,0,0,0), relative=True)
# except RobotError, ex:
#     print("Robot could not execute move (emergency stop for example), do something", ex)






# def testHz_getPose():
#     pose = rob.getl()
# print pose


# count=0
# time_time0 = time.time()
# cost=0
# while(cost<=1):
#     testHz_getPose()
#     cost=time.time()-time_time0
#     count+=1
# print count

# rob = urx.Robot("192.168.100.2")
# rob.set_tcp((0,0,0,0,0,0))
# rob.set_payload(0.5, (0,0,0))
# try:
#     l = 0.05
#     v = 0.01
#     a = 0.1
#     pose = rob.getl()           # x,y,z,rx,ry,rz
#     print("robot tcp is at: ", pose)
#     print("absolute move in base coordinate ")
#     pose[2] += l
#     rob.movel(pose, acc=a, vel=v)
#     # print("relative move in base coordinate ")
#     # rob.translate((0, 0, -l), acc=a, vel=v)
#     # print("relative move back and forth in tool coordinate")
#     # rob.translate_tool((0, 0, -l), acc=a, vel=v)
#     # rob.translate_tool((0, 0, l), acc=a, vel=v)
# finally:
#     rob.close()