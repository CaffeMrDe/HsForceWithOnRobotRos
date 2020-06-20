#include <iostream>
#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include "Python.h"
using namespace std;
int main(int args,char** argv){
    //初始化python
    Py_Initialize();
    //引入当前路径,否则下面模块不能正常导入
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/wangneng/catkin_ws_handRob/src/HsForceWithOnRobotRos/impedance_control/scripts')");
    // 声明变量
    PyObject* pModule = nullptr;
    PyObject* pFunc = nullptr;
    PyObject *pDict= nullptr;
    PyObject * pArgs= nullptr;
    PyObject * pRetVal= nullptr;
    pModule = PyImport_ImportModule("ur5_drive");
    //获取模块字典属性
    pDict = PyModule_GetDict(pModule);
    //通过字典属性获取模块中的类
    pFunc = PyDict_GetItemString(pDict, "ur_getpose");
    //调用函数，并得到python类型的返回值
    int a[6];
    pArgs = PyTuple_New(6);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 11));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 11));
    PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", 11));
    PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", 11));
    PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", 11));
    PyTuple_SetItem(pArgs, 5, Py_BuildValue("i", 11));
    pRetVal= PyObject_CallObject(pFunc, pArgs);
    PyArg_ParseTuple(pRetVal, "iiiiii",&a[0],&a[1],&a[2],&a[3],&a[4],&a[5]);
//    PyArg_ParseTuple(pRetVal, "ii",&a1,&a2);
    //释放python
    Py_Finalize();
    return 0;
}


//    // 移动到up点
//    string groupName = "manipulator";
//    moveit::planning_interface::MoveGroupInterface group(groupName);
//    group.setNamedTarget("up");
//    group.move();

//
//// 移动到home点
//    string groupName = "manipulator";
//    moveit::planning_interface::MoveGroupInterface group(groupName);
//    group.setNamedTarget("home");
//    group.move();


//移动到随机位置
//    string groupName = "manipulator";
//    moveit::planning_interface::MoveGroupInterface group(groupName);
//    group.setRandomTarget();
//    group.move();

//    移动到指定位置
//    string groupName = "manipulator";
//    moveit::planning_interface::MoveGroupInterface group(groupName);
//
//    geometry_msgs::Pose pose;
////位置
//    pose.position.x = 0.28;
//    pose.position.y = 0.2;
//    pose.position.z = 0.5;
////姿态
//    pose.orientation.x = 0;
//    pose.orientation.y = 0;
//    pose.orientation.z = 0;
//    pose.orientation.w = 1;
//    group.setPoseTarget(pose);
//    group.move();


//    C++欧拉角转四元素
//// 导入库
//#include "tf/LinearMath/Quaternion.h"
//
//// 转换
//    tf::Quaternion quat;
//    quat.setRPY(roll, pitch, yaw);
//
////　使用
//    double x = quat.x();
//    double y = quat.y();
//    double z = quat.z();
//    double w = quat.w();

//移动结果判断
//    const moveit::planning_interface::MoveItErrorCode &code = group.move();
//    if (code == code.SUCCESS) {
//        // success
//    } else if(code == code.PLANNING_FAILED) {
//        // failed
//    }


////    通过字典属性获取模块中的类
//PyObject *pClass= nullptr;
//pClass = PyDict_GetItemString(pDict, "UR5_Move");
////实例化获取的类
//PyObject *pInstance= nullptr;
//pInstance = PyInstance_New(pClass, nullptr, nullptr);
////    //调用类的方法
////调用函数，并得到python类型的返回值
//PyObject *pRetVal= nullptr;
//char* ret_string;
//pRetVal = PyObject_CallMethod(pInstance, (char*)"ur_getpose", nullptr);
//PyArg_Parse(pRetVal, "s", &ret_string);
//cout<<ret_string<<endl;
////    PyObject_CallMethod(pInstance, (char*)"ur_close", nullptr);
////
//pArgs = PyTuple_New(6);
//PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 11));
//PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 11));
//PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", 11));
//PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", 11));
//PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", 11));
//PyTuple_SetItem(pArgs, 5, Py_BuildValue("i", 11));
//PyObject_CallMethod(pInstance, (char*)"ur_moveL", "s","sdasd");
