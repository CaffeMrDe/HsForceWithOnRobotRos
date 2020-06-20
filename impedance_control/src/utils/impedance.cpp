#include "impedance.h"

impedance::impedance(ros::NodeHandle* node):Node(node) {
    //控制阻抗模块启动或停止
    SubCommand_begin=Node->subscribe<std_msgs::Bool>("ShakeHandRobot_ctrlCommand",1,&impedance::Subcallback_begin,this);
    //接收力矩传感器数据
    SubForceSensor=Node->subscribe<geometry_msgs::Wrench>("daq_data",1000,&impedance::Subcallback_ForceSensor,this);
    //发布机器人目标执行点
    PubPose_moveRob=Node->advertise<std_msgs::Float32MultiArray>("Pose_moveRob", 1);
    Pub_CurPose=Node->advertise<std_msgs::Float64MultiArray>("/ur_curPose", 1);
    Pub_NextPose=Node->advertise<std_msgs::Float64MultiArray>("/ur_nextPose", 1);
    Sub_workOnce=Node->subscribe<std_msgs::Bool>("/ur_workOnce",1,&impedance::Subcallback_workOnce,this);
    //创建定时器
    pTimer_bringup = new CTimer("pTimer_bringup");
    pTimer_getRobPose = new CTimer("pTimer_getRobPose");
    pTimer_shakeHandCount = new CTimer("pTimer_shakeHandCount");
//    hsrobot=new hsRobotUtil();
//    flag_connRob = hsrobot->connectRobot();
//    if(!flag_connRob){
//        cout<<"机器人未连接"<<endl;
//    }
    py_ur5_init();
}


//启动或停止命令
void impedance::Subcallback_begin(std_msgs::Bool msg) {
    //开始启动命令
    if(msg.data){
        cout<<"启动"<<endl;
//        pTimer_bringup->AsyncLoop(SLEEP_TIME,&impedance::forecastNextPose,this);//频率为250hz
        pTimer_bringup->AsyncLoop(10000,&impedance::forecastNextPose,this);//频率为250hz
        pTimer_getRobPose->AsyncLoop(SLEEP_TIME,&impedance::Thread_RecvRobPose,this);//频率为250hz
    }
    //停止命令
    else{
        pTimer_bringup->Cancel();
        pTimer_getRobPose->Cancel();
        py_ur5_close();
    }
}
//获取机器人当前点位数据
void impedance::Thread_RecvRobPose() {
//    if(!flag_connRob){
//        return;
//    }
//    cout<<"获取机器人当前点位数据"<<endl;
    py_ur5_getpose();
//    mutex_shareRobPose.lock();
//    hsrobot->getRobotCurPos(posM_robCurPose);
//    mutex_shareRobPose.unlock();
//    cout<<posM_robCurPose.data[0]<<"--"<<posM_robCurPose.data[1]<<"--"<<posM_robCurPose.data[2]<<endl;
//    cout<<posM_robCurPose.data[3]<<"--"<<posM_robCurPose.data[4]<<"--"<<posM_robCurPose.data[5]<<endl;
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

    //读出的数据单位是mm
    mutex_shareRobPose.lock();
    for (int k = 0; k < 6; ++k) {
//        robCurPose[k]=posM_robCurPose.data[k];
        robCurPose[k]=ur_robCurPose[k];
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
//    for (int i = 0; i < 6; ++i) {
//        robNextPose[i]=robCurPose[i]+(curVel[i]*T+0.5*acc[i]*T*T)*1000;
//    }
    for (int i = 0; i < 6; ++i) {
        robNextPose[i]=robCurPose[i];
    }
    robNextPose[2]+=0.01;
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
    std_msgs::Float64MultiArray msg;
    msg.data.resize(6);
    for (int j = 0; j < 6; ++j) {
        msg.data[j]=robNextPose[j];
    }
//    PubPose_moveRob.publish(msg);
    Pub_NextPose.publish(msg);
//    cout<<"发布中"<<endl;
//    if(!flag_connRob){
//        return;
//    }
//    copy(begin(posM_robNextPose.data),begin(posM_robNextPose.data)+6,robNextPose);
//    hsrobot->setRobotPosition(posM_robNextPose);
    py_ur5_movel(robNextPose);
}

//计算握手次数
void impedance::culculate_shakeHand() {
    return;
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
        sh_data={true,{0},{0},0,0,0};
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
    //5.记录握手时间,周期是0.004s
    sh_data.SH_time++;
    //如果10s钟没有完成一次握手,则重新计算
    if(sh_data.SH_time>250*10){
        //数据初始化
        sh_data={false,{0},{0},0,0,0};
        flag_statCount= false;
        pTimer_shakeHandCount->Cancel();
    }
    //6.判断是否完成一次握手
    if(sh_data.MaxDistance>50&&sh_data.MinDistance<10){
        count_shakeHand++;
        //数据初始化
        sh_data={false,{0},{0},0,0,0};
        flag_statCount= false;
        pTimer_shakeHandCount->Cancel();
    }
}

void impedance::py_ur5_init() {
    cout<<"py_ur5_init--start"<<endl;
    //初始化python
    Py_Initialize();
    //引入当前路径,否则下面模块不能正常导入
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/wangneng/catkin_ws_handRob/src/HsForceWithOnRobotRos/impedance_control/scripts')");
    pModule = PyImport_ImportModule("ur5_drive");
    if(pModule== NULL){
        cout<<"pModule null"<<endl;
    }
    //获取模块字典属性
    cout<<"1"<<endl;
    pDict = PyModule_GetDict(pModule);
    if(pDict== nullptr){
        cout<<"pDict null"<<endl;
    }
    cout<<"2"<<endl;
    //通过字典属性获取模块中的函数
    pFunc_ur_init = PyDict_GetItemString(pDict, "ur_init");
    pFunc_getpose = PyDict_GetItemString(pDict, "ur_getpose");
    pFunc_ur_moveL = PyDict_GetItemString(pDict, "ur_moveL");
    pFunc_ur_close = PyDict_GetItemString(pDict, "ur_close");

    PyObject_CallObject(pFunc_ur_init, NULL);
    cout<<"py_ur5_init--end"<<endl;
    pArgs_move = PyTuple_New(6);
}


void impedance::py_ur5_getpose() {
    mutex_forCallPy.lock();
    pRetVal= PyObject_CallObject(pFunc_getpose, NULL);
    mutex_shareRobPose.lock();
    PyArg_ParseTuple(pRetVal, "dddddd",&ur_robCurPose[0],&ur_robCurPose[1],&ur_robCurPose[2],&ur_robCurPose[3],&ur_robCurPose[4],&ur_robCurPose[5]);
    Py_XDECREF(pRetVal);
    mutex_shareRobPose.unlock();
    mutex_forCallPy.unlock();

    std_msgs::Float64MultiArray msg;
    msg.data.resize(6);
    for (int j = 0; j < 6; ++j) {
        msg.data[j]=ur_robCurPose[j];
    }
    Pub_CurPose.publish(msg);
//    cout<<ur_robCurPose[0]<<"--"<<ur_robCurPose[1]<<"--"<<ur_robCurPose[2]<<endl;
//    cout<<ur_robCurPose[3]<<"--"<<ur_robCurPose[4]<<"--"<<ur_robCurPose[5]<<endl;

}



void impedance::py_ur5_movel(double * pose) {
        test_count++;
        cout<<"py_ur5_movel--start"<<endl;
    mutex_forCallPy.lock();
//    double tmp_pose[6]{55};
//    PyObject *obj1 = Py_BuildValue("d",77.77);
//    PyObject *obj2 = Py_BuildValue("d",77.77);
//    PyObject *obj3 = Py_BuildValue("d",77.77);
//    PyObject *obj4 = Py_BuildValue("d",77.77);
//    PyObject *obj5 = Py_BuildValue("d",77.77);
//    PyObject *obj6 = Py_BuildValue("d",77.77);
    PyTuple_SetItem(pArgs_move, 0, Py_BuildValue("d", pose[0]));
    PyTuple_SetItem(pArgs_move, 1, Py_BuildValue("d", pose[1]));
    PyTuple_SetItem(pArgs_move, 2, Py_BuildValue("d", pose[2]));
    PyTuple_SetItem(pArgs_move, 3, Py_BuildValue("d", pose[3]));
    PyTuple_SetItem(pArgs_move, 4, Py_BuildValue("d", pose[4]));
    PyTuple_SetItem(pArgs_move, 5, Py_BuildValue("d", pose[5]));
//    cout<<i1<<i2<<i3<<i4<<i5<<i6<<endl;
    PyObject_CallObject(pFunc_ur_moveL, pArgs_move);
//    PyObject_CallObject(pFunc_ur_moveL, obj1,obj2,obj3,obj4,obj5,obj6);
//    PyObject_CallObject(pFunc_ur_moveL, obj1,obj2,obj3,obj4,obj5,obj6);
//    PyObject_CallFunction(pFunc_ur_moveL, (char *) "O,O,O,O,,O,O", obj1, obj2, obj3, obj4, obj5,obj6);
    mutex_forCallPy.unlock();
    cout<<"py_ur5_movel--end"<<endl;
//    Py_XDECREF(obj1);
//    Py_XDECREF(obj2);
//    Py_XDECREF(obj3);
//    Py_XDECREF(obj4);
//    Py_XDECREF(obj5);
//    Py_XDECREF(obj6);
    cout<<test_count<<endl;
}

void impedance::py_ur5_close() {
    cout<<"py_ur5_close--start"<<endl;
    PyObject_CallObject(pFunc_ur_close, nullptr);
    //释放python
    Py_Finalize();
    cout<<"py_ur5_close--end"<<endl;
}

void impedance::Subcallback_workOnce(std_msgs::Bool msg) {
    mutex_shareRobPose.lock();
    for (int k = 0; k < 6; ++k) {
//        robCurPose[k]=posM_robCurPose.data[k];
        robCurPose[k]=ur_robCurPose[k];
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
//    for (int i = 0; i < 3; ++i) {
//        robNextPose[i]=robCurPose[i]+(curVel[i]*T+0.5*acc[i]*T*T)*1000;
//    }
//    for (int i = 3; i < 6; ++i) {
//        robNextPose[i]=robCurPose[i]+(curVel[i]*T+0.5*acc[i]*T*T);
//    }

//    for (int i = 0; i < 6; ++i) {
//        robNextPose[i]=robCurPose[i];
//    }
//    robNextPose[0]+=1;


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
    std_msgs::Float64MultiArray data_msg;
    data_msg.data.resize(6);
    for (int j = 0; j < 6; ++j) {
        data_msg.data[j]=robNextPose[j];
    }
    Pub_NextPose.publish(data_msg);

    py_ur5_movel(robNextPose);
}
