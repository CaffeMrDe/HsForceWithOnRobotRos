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
#include "geometry_msgs/Wrench.h"
#include "hsRobotUtil.h"
using namespace std;

#define SLEEP_TIME 3800

enum RW_level {READ=0,WRITE};

struct shakeHand{
    bool recordInitPose= false;
    double initPose[3];
    double CurPose[3];
    double MaxDistance;
    double MinDistance;
};

class impedance {
public:
    impedance(ros::NodeHandle* node);
private:
    //M参数 轴数对应:X,Y,Z,Pitch,Yaw,Roll
    const float M_param[6]{1.6,1.6,3.2,0.4,0.3,0.2};
    //B参数
    const float B_param[6]{4.8,4.8,3.6,1.6,1.2,0.6};
    //当前机器人欧拉角坐标
    posMotion posM_robCurPose;
    float robCurPose[6];
    //下一步欧拉角坐标预测值
    float robNextPose[6];
    posMotion posM_robNextPose;
    //力矩传感器数据
    float forceSensor[6];
    //是当前速度
    float curVel[6]{0};
    //插补周期
    float T=0.004;
    hsRobotUtil hsrobot;
    CTimer *pTimer_bringup;
    CTimer *pTimer_getRobPose;
    CTimer *pTimer_shakeHandCount;
    CTimer *pTimer_10S;
    mutex mutex_shareRobPose;
    mutex mutex_shareForceSensor;
    bool flag_connRob= false;
    bool flag_statCount= false;
    shakeHand sh_data;
    int count_shakeHand=0;

    //ros对象
    ros::NodeHandle* Node;
    ros::Publisher PubPose_moveRob;//发布机器人下一步执行运动点位
    ros::Subscriber SubRobPose;    //接收机器人当前点位数据
    ros::Subscriber SubCommand_begin; //接受启动和停止命令
    ros::Subscriber SubForceSensor;  //接受传感器数据

private:
    /***
     * 根据当前机器人坐标数据与力矩传感器数据预测下一步机器人坐标,0.004s发布一次
     * @return
     */
    void forecastNextPose();

    /***
     * 接收开启或停止命令
     * @param msg
     */
    void Subcallback_begin(std_msgs::Bool msg);

    /***
     *  获取机器人数据线程函数
     */
    void Thread_RecvRobPose();

    /***
     * 接收力矩传感器的数据
     * @param msg
     */
    void Subcallback_ForceSensor(geometry_msgs::Wrench msg);

    /***
     * 计算握手次数
     */
    void culculate_shakeHand();


};


#endif //IMPEDANCE_CONTROL_IMPEDANCE_H
