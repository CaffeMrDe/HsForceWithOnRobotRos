#ifndef IMPEDANCE_CONTROL_IMPEDANCE_H
#define IMPEDANCE_CONTROL_IMPEDANCE_H

#include <iostream>
#include <vector>
#include<sys/time.h>

using namespace std;

class BaseTimerTask
{
public:
    BaseTimerTask():m_uiTvSec(0),m_uiTvUsec(0){}
    virtual ~BaseTimerTask(){}
public:
    //设置定时器间隔
    void setTimer(uint uiTvSec,uint uiTvUsec = 0);

    //启动定时器
    void startTimer(uint uiTvSec,uint uiTvUsec = 0);

    //周期性任务
    virtual void intervalTask()= 0;

private:
    uint m_uiTvSec;           //秒
    uint m_uiTvUsec;          //微秒
};


class impedance {
private:
    //M参数
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
private:
    //发布下一点坐标
    void pubNextPose();
    //根据当前机器人坐标数据与力矩传感器数据预测下一步机器人坐标,0.004s发布一次
    //阻抗控制 f(t)=ma+bv
    float* forecastNextPose();

};


#endif //IMPEDANCE_CONTROL_IMPEDANCE_H
