#include "Ctimer.h"

#include <future>

CTimer::CTimer(const std::string sTimerName):m_bExpired(true), m_bTryExpired(false), m_bLoop(false)
{
    m_sName = sTimerName;
}

CTimer::~CTimer()
{
    m_bTryExpired = true;   //尝试使任务过期
    DeleteThread();
}

bool CTimer::Start(unsigned int msTime, std::function<void()> task, bool bLoop, bool async)
{
    if (!m_bExpired || m_bTryExpired) return false;  //任务未过期(即内部仍在存在或正在运行任务)
    m_bExpired = false;
    m_bLoop = bLoop;
    m_nCount = 0;

    if (async) {
        cout<<"异步"<<endl;
        DeleteThread();
        cout<<"创建子线程"<<endl;
        m_Thread = new std::thread([this, msTime, task]() {
            cout<<"进入lamda函数"<<endl;
            while (!m_bTryExpired) {
                cout<<"任务没过期1"<<endl;
                m_ThreadCon.wait_for(m_ThreadLock, std::chrono::milliseconds(msTime));  //休眠
                if (!m_bTryExpired) {
                    cout<<"任务没过期2"<<endl;
                    task();     //执行任务

                    m_nCount ++;
                    if (!m_bLoop) {
                        cout<<"结束循环"<<endl;
                        break;
                    }
                }
            }

            m_bExpired = true;      //任务执行完成(表示已有任务已过期)
            m_bTryExpired = false;  //为了下次再次装载任务
        });
    } else {
        cout<<"不是异步"<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(msTime));
        if (!m_bTryExpired) {
            cout<<"任务没过期"<<endl;
            task();
        }
        m_bExpired = true;
        m_bTryExpired = false;
    }

    return true;
}

void CTimer::Cancel()
{
    if (m_bExpired || m_bTryExpired || !m_Thread) {
        return;
    }

    m_bTryExpired = true;
}

void CTimer::DeleteThread()
{
    if (m_Thread) {
        m_ThreadCon.notify_all();   //休眠唤醒
        m_Thread->join();           //等待线程退出
        delete m_Thread;
        m_Thread = nullptr;
    }
}
