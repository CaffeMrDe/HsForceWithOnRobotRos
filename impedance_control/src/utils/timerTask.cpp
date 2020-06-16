
#include "timerTask.h"

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

/********************************************************
BaseTimerTask
*********************************************************/

/********************************************************
   Func Name: setTimer
Date Created: 2018-7-30
 Description: 设置定时器
       Input: uiTvSec：设置间隔多少秒
             uiTvUsec：设置间隔多少微秒
      Output:
      Return:
     Caution:
*********************************************************/
void BaseTimerTask::setTimer(uint uiTvSec,uint uiTvUsec)
{
    m_uiTvSec = uiTvSec;
    m_uiTvUsec = uiTvUsec;
}

/********************************************************
   Func Name: setTimer
Date Created: 2018-7-30
 Description: 开启定时器
       Input: uiTvSec：设置间隔多少秒
             uiTvUsec：设置间隔多少微秒
      Output:
      Return:
     Caution:
*********************************************************/
void BaseTimerTask::startTimer(uint uiTvSec,uint uiTvUsec)
{
    int infds = 0;
    struct timeval stTimeval;
    m_uiTvSec = uiTvSec;
    m_uiTvUsec = uiTvUsec;

    do
    {
        stTimeval.tv_sec = (long)m_uiTvSec;
        stTimeval.tv_usec = (long)m_uiTvUsec;
        //select()每次返回后都会清空struct timeval参数
        infds = select(0, nullptr, nullptr, nullptr, &stTimeval);
        //执行任务
        intervalTask();
    } while ((0 == infds) || (infds < 0 && EINTR == errno));
}

/********************************************************
CPrintTimer
*********************************************************/

/********************************************************
   Func Name: intervalTask
Date Created: 2018-7-30
 Description: 执行任务
       Input:
      Output:
      Return:
     Caution:
*********************************************************/
void CPrintTimer::intervalTask()
{
    cout << "praise chaos" << endl;
}