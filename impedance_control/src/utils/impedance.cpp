#include "impedance.h"

impedance::impedance(ros::NodeHandle* node):Node(node) {
    //控制阻抗模块启动或停止
    SubCommand_begin=Node->subscribe<std_msgs::Bool>("ShakeHandRobot_ctrlCommand",1,&impedance::Subcallback_begin,this);
    //接收力矩传感器数据
    SubForceSensor=Node->subscribe<geometry_msgs::Wrench>("daq_data",1000,&impedance::Subcallback_ForceSensor,this);
    //发布机器人目标执行点
    PubPose_moveRob=Node->advertise<std_msgs::Float32MultiArray>("Pose_moveRob", 1);
    //创建定时器
    pTimer_bringup = new CTimer("pTimer_bringup");
    pTimer_getRobPose = new CTimer("pTimer_getRobPose");
    pTimer_shakeHandCount = new CTimer("pTimer_shakeHandCount");
    pTimer_10S = new CTimer("pTimer_10S");
    flag_connRob = hsrobot.connectRobot();
    if(!flag_connRob){
        cout<<"机器人未连接"<<endl;
    }
}

//启动或停止命令
void impedance::Subcallback_begin(std_msgs::Bool msg) {
    //开始启动命令
    if(msg.data){
        cout<<"启动"<<endl;
        pTimer_bringup->AsyncLoop(SLEEP_TIME,&impedance::forecastNextPose,this);//频率为250hz
        pTimer_getRobPose->AsyncLoop(SLEEP_TIME,&impedance::Thread_RecvRobPose,this);//频率为250hz
    }
    //停止命令
    else{
        pTimer_bringup->Cancel();
    }
}
//获取机器人当前点位数据
void impedance::Thread_RecvRobPose() {
    if(!flag_connRob){
        return;
    }
    cout<<"获取机器人当前点位数据"<<endl;
    mutex_shareRobPose.lock();
    hsrobot.getRobotCurPos(posM_robCurPose);
    mutex_shareRobPose.unlock();
}

//接收力矩传感器数据
void impedance::Subcallback_ForceSensor(geometry_msgs::Wrench msg) {
    cout<<"获取力传感器数据"<<endl;
        mutex_shareForceSensor.lock();
        forceSensor[0]=msg.force.x;
        forceSensor[1]=msg.force.y;
        forceSensor[2]=msg.force.z;
        forceSensor[3]=msg.torque.x;
        forceSensor[4]=msg.torque.y;
        forceSensor[5]=msg.torque.z;
        mutex_shareForceSensor.unlock();
        //任意XYZ方向受力大于1N,表示有效握手开始,并计数
        if((abs(forceSensor[0])>1)||(abs(forceSensor[1])>1)||(abs(forceSensor[2])>1)){
            pTimer_shakeHandCount->AsyncLoop(SLEEP_TIME,&impedance::culculate_shakeHand,this);
        }
}

void impedance::forecastNextPose() {
    mutex_shareRobPose.lock();
    for (int k = 0; k < 6; ++k) {
        robCurPose[k]=posM_robCurPose.data[k];
    }
    mutex_shareRobPose.unlock();
    //加速度计算
    float acc[6];
    mutex_shareForceSensor.lock();
    for (int i = 0; i < 6; ++i) {
        acc[i]=(forceSensor[i]-B_param[i]*curVel[i])/M_param[i];
    }
    mutex_shareForceSensor.unlock();
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
        curVel[i]=curVel[i]+acc[i]*T;
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
    PubPose_moveRob.publish(msg);
//    cout<<"发布中"<<endl;
    if(!flag_connRob){
        return;
    }
    copy(begin(posM_robNextPose.data),begin(posM_robNextPose.data)+6,robNextPose);
    hsrobot.setRobotPosition(posM_robNextPose);
}

//计算握手次数
void impedance::culculate_shakeHand() {
    //1.接受开始计算命令(表示有外力数据输入)
    if(!flag_statCount){
        return;
    }
    //2.采集机器人当前数据
    double tmp_RbPose[3]{0};
    mutex_shareRobPose.lock();
    copy(begin(posM_robCurPose.data),begin(posM_robCurPose.data)+3,tmp_RbPose);
    mutex_shareRobPose.unlock();
    //3.记录初值并初始化
    if(!sh_data.recordInitPose){
        sh_data={true,{0},{0},0,0};
        copy(begin(tmp_RbPose),begin(tmp_RbPose)+3,sh_data.initPose);
        copy(begin(tmp_RbPose),begin(tmp_RbPose)+3,sh_data.CurPose);
    }
    //4.记录最大距离或最小值
    double tmp_Distance=pow((tmp_RbPose[0]-sh_data.CurPose[0]),2)+pow((tmp_RbPose[1]-sh_data.CurPose[1]),2)+pow((tmp_RbPose[2]-sh_data.CurPose[2]),2);
    copy(begin(tmp_RbPose),begin(tmp_RbPose)+3,sh_data.CurPose);
    if(tmp_Distance>sh_data.MaxDistance){
        sh_data.MaxDistance=tmp_Distance;
        sh_data.MinDistance=tmp_Distance;
    } else{
        if(tmp_Distance<sh_data.MinDistance){
            sh_data.MinDistance=tmp_Distance;
        }
    }
    //5.判断是否完成一次握手
    if(sh_data.MaxDistance>50&&sh_data.MinDistance<10){
        count_shakeHand++;
        //数据初始化
        sh_data={false,{0},{0},0,0};
        flag_statCount= false;
        pTimer_shakeHandCount->Cancel();
    }

}