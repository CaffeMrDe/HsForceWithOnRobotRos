#pragma once
#include "CommApi.h"
#include "proxy/ProxyVar.h"
#include "proxy/ProxyMotion.h"
#include "proxy/ProxyMotion.h"
#include "memory"


using namespace Hsc3::Comm;
using namespace Hsc3::Proxy;
#define IPADDR "10.10.56.214"
#define PORT 23234

struct posMotion{
    double data[6];
};

class hsRobotUtil
{
public:
    hsRobotUtil();
    /**
     * @brief connectRobot
     * @return
     */
    bool connectRobot();

    /**
     * @brief setRobotPosition
     * @param pdata
     */
    void setRobotPosition(const posMotion &pdata);

    /**
     * @brief getRobotCurPos
     * @param pdata
     */
    void getRobotCurPos(posMotion &pdata);
private:
    bool getConnect();
    std::shared_ptr<CommApi>  commn;
    std::shared_ptr<ProxyVar>  proxyV;
    std::shared_ptr<ProxyMotion>  proxyMotion;
};
