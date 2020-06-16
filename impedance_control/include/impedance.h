#ifndef IMPEDANCE_CONTROL_IMPEDANCE_H
#define IMPEDANCE_CONTROL_IMPEDANCE_H

#include <iostream>
#include <vector>
#include<sys/time.h>
#include <thread>
#include <mutex>
#include "Ctimer.h"
#include "ros/ros.h"

#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Bool.h"
using namespace std;

enum RW_level {READ=0,WRITE};

class impedance {
public:
    impedance(ros::NodeHandle* node);
private:
    //M参数 轴数对应:X,Y,Z,Pitch,Yaw,Roll
    const float M_param[6]{1.6,1.6,3.2,0.4,0.3,0.2};
    //B参数
    const float B_param[6]{4.8,4.8,3.6,1.6,1.2,0.6};
    //当前机器人欧拉角坐标
    float robCurPose[6];
    //下一步欧拉角坐标预测值
    float robNextPose[6];
    //力矩传感器数据
    float forceSensor[6];
    //是当前速度
    float curVel[6]{0};
    //插补周期
    float T=0.004;
    //加锁后拷贝数据
    float lock_robCurPose[6];
    CTimer *pTimer;
    thread *m_Thread = nullptr;
    mutex mutex1;
    bool flag_start= false;

    //ros对象
    ros::NodeHandle* Node;
    ros::Publisher PubPose_moveRob;//发布机器人下一步执行运动点位
    ros::Subscriber SubRobPose;    //接收机器人当前点位数据
    ros::Subscriber SubCommand_begin; //接受启动和停止命令
    ros::Subscriber SubForceSensor;  //接受传感器数据

private:
    //根据当前机器人坐标数据与力矩传感器数据预测下一步机器人坐标,0.004s发布一次
    float* forecastNextPose();

    void Subcallback_begin(std_msgs::Bool msg);

    void Subcallback_ReceivePose(std_msgs::Float32MultiArray msg);

    void Subcallback_ForceSensor(std_msgs::Float32MultiArray msg);

    void test_thread();

    //读写机器人当前位置时加锁
    float* readAndWrite_robCurPose(RW_level lev,float* data= nullptr);

    void test_read();

    void test_write();

};


#endif //IMPEDANCE_CONTROL_IMPEDANCE_H
