#ifndef IMPEDANCE_CONTROL_TIMERTASK_H
#define IMPEDANCE_CONTROL_TIMERTASK_H

#include <iostream>
//#include "asr_taskinterface.h"
//#include "comontype.h"
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
    virtual void intervalTask() = 0;

private:
    uint m_uiTvSec;           //秒
    uint m_uiTvUsec;          //微秒
};


class CPrintTimer:public BaseTimerTask
{
public:
    virtual void intervalTask();
};


#endif //IMPEDANCE_CONTROL_TIMERTASK_H
