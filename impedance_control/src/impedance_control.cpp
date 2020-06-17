#include <iostream>
#include "ros/ros.h"
#include "impedance.h"
using namespace std;


#include <hsRobotUtil.h>
int main(int args,char** argv){
    //ros节点
    std::string nodeName = "impedance_control";
    ros::init(args, argv, nodeName);
    //创建节点
    ros::NodeHandle node;
    //初始化阻抗控制模块
    impedance imp(&node);

    ros::spin();
    return 0;
}


