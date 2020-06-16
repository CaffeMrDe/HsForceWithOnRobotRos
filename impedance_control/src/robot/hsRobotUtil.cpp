#include "hsRobotUtil.h"
#include "vector"
#include "algorithm"
hsRobotUtil::hsRobotUtil() {
        commn = std::make_shared<CommApi>();
        proxyV = std::make_shared<ProxyVar>(commn.get());
        proxyMotion = std::make_shared<ProxyMotion>(commn.get());
}

void hsRobotUtil::setRobotPosition(const posMotion &pdata)
{

    LocPos p;
    p.ufNum = -1;
    p.utNum = -1;
    p.vecPos.reserve(6);
    p.vecPos.assign(&pdata.data[0], &pdata.data[6]);
    proxyV->setLR(0, -1, p);
}

void hsRobotUtil::getRobotCurPos(posMotion &pdata)
{
   std::vector<double> cpdata;
   proxyMotion->getLocData(0,cpdata);
   std::copy(cpdata.begin() ,cpdata.begin() +6, pdata.data);
}

bool hsRobotUtil::connectRobot()
{
    int ret = commn->connect(IPADDR,PORT);
    return ret == 0? true :false;
}

bool hsRobotUtil::getConnect()
{
    int ret =commn->isConnected();
    if(ret != 0){
        return connectRobot();
    }
    return ret == 0? true :false;
}
