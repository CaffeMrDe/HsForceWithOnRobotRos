#include "../../include/impedance.h"











void impedance::pubNextPose() {

}

float* impedance::forecastNextPose() {
    //加速度预测量
    float acc[6];
    for (int i = 0; i < 6; ++i) {
        acc[i]=(forceSensor[i]-B_param[i]*curVel[i])/M_param[i];
    }
    //预测点位为
    for (int i = 0; i < 6; ++i) {
        robNextPose[i]=robCurPose[i]+curVel[i]*T+0.5*acc[i]*T*T;
    }
    //速度更新? 还是改为平均速度更新
    for (int i = 0; i < 6; ++i) {
        curVel[i]=acc[i]*T;
    }
}