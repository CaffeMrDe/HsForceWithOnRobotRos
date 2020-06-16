#include <iostream>
#include "ros/ros.h"
#include "timerTask.h"
#include "Ctimer.h"
using namespace std;



int main(int args,char** argv){

    int a = 1;
    int b = 2;
    int sum = 0;
    CTimer *pTimer = new CTimer("定时器4");
    bool loop = pTimer->AsyncLoop(1000, [](int a, int b, int *sum) {
        *sum = a + b + *sum;
        printf("累加 sum += a + b + *sum: %d\n", *sum);
    }, a, b, &sum);    //异步循环执行，间隔时间1秒
//    //ros节点
//    std::string nodeName = "impedance_control";
//    ros::init(args, argv, nodeName);
//    //创建节点
//    ros::NodeHandle node;
//    ros::spin();
    return 0;
}
