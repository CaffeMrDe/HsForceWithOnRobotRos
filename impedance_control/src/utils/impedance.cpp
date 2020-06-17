#include "impedance.h"

impedance::impedance(ros::NodeHandle* node):Node(node) {
    //控制阻抗模块启动或停止
    SubCommand_begin=Node->subscribe<std_msgs::Bool>("imp_begin",1,&impedance::Subcallback_begin,this);
    //接收机器人当前位置点
    SubRobPose=Node->subscribe<std_msgs::Float32MultiArray>("Pose_receive",1,&impedance::Subcallback_ReceivePose,this);
    //接收力矩传感器数据
    SubForceSensor=Node->subscribe<std_msgs::Float32MultiArray>("data_ForceSensor",1,&impedance::Subcallback_ForceSensor,this);
    //发布机器人目标执行点
    PubPose_moveRob=Node->advertise<std_msgs::Float32MultiArray>("Pose_moveRob", 1);
    //创建定时器
    pTimer = new CTimer("timer");
    m_Thread=new thread(&impedance::test_thread,this);

}

//启动或停止命令
void impedance::Subcallback_begin(std_msgs::Bool msg) {
    if(msg.data){
        pTimer->AsyncLoop(3780,&impedance::forecastNextPose,this);//频率为250hz
    } else{
        pTimer->Cancel();
    }
}
//接收机器人当前点位数据
void impedance::Subcallback_ReceivePose(std_msgs::Float32MultiArray msg) {
    for (int i = 0; i < msg.data.size(); ++i) {
        robCurPose[i]=msg.data[i];
    }
}

//接收力矩传感器数据
void impedance::Subcallback_ForceSensor(std_msgs::Float32MultiArray msg) {
    for (int i = 0; i < msg.data.size(); ++i) {
        forceSensor[i]=msg.data[i];
    }
}

float* impedance::forecastNextPose() {
    //加速度计算
    float acc[6];
    for (int i = 0; i < 6; ++i) {
        acc[i]=(forceSensor[i]-B_param[i]*curVel[i])/M_param[i];
    }
    //加速度限制
    for (int i = 0; i < 6; ++i){
        if(abs(acc[i])>0.5){
            if(acc[i]>0){
                acc[i]=0.5;
            } else{
                acc[i]=-0.5;
            }
        }
    }
    //预测点位为
    for (int i = 0; i < 6; ++i) {
        robNextPose[i]=robCurPose[i]+curVel[i]*T+0.5*acc[i]*T*T;
    }
    //速度更新? 还是改为平均速度更新
    for (int i = 0; i < 6; ++i) {
        curVel[i]=acc[i]*T;
    }
    //速度限制
    for (int i = 0; i < 3; ++i){
        if(abs(curVel[i])>0.5){
            if(curVel[i]>0){
                curVel[i]=0.5;
            } else{
                curVel[i]=-0.5;
            }
        }
    }
    for (int i = 3; i < 6; ++i){
        if(abs(curVel[i])>0.8){
            if(curVel[i]>0){
                curVel[i]=0.8;
            } else{
                curVel[i]=-0.8;
            }
        }
    }
    std_msgs::Float32MultiArray msg;
    msg.data.resize(6);
    for (int j = 0; j < 6; ++j) {
        msg.data[j]=robNextPose[j];
    }
    cout<<"发布中"<<endl;
    PubPose_moveRob.publish(msg);
}

//测试线程,发布频率为250HZ
void impedance::test_thread() {
    while (true){
        usleep(1000*1000);
        while (flag_start){
            usleep(3780);
            forecastNextPose();
        }
    }
}

float *impedance::readAndWrite_robCurPose(RW_level lev,float* data) {
    mutex1.lock();
    if(lev==RW_level::READ){
        copy(begin(robCurPose),end(robCurPose),lock_robCurPose);
    }
    if(lev==RW_level::WRITE){
        for (int i = 0; i < 6; ++i) {
            robCurPose[i]=data[i];
        }
    }

    return nullptr;
}

////测试多线程读写数据问题,当读写过快时会出现数据错误现象,需要加锁
//void impedance::test_read() {
//    while(true)
//    {
//        usleep(2);
//        if(robCurPose[0]!=robCurPose[1]){
//            cout<<"读入错误"<<endl;
//            cout<<robCurPose[0]<<"--"<<robCurPose[1]<<"--"<<robCurPose[2]<<"--"<<robCurPose[3]<<"--"<<robCurPose[4]<<"--"<<robCurPose[5]<<"--"<<endl;
//        }





