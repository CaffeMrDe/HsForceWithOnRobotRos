#include "MainWindow.h"
#include <iostream>
using namespace std;



int main(int args,char** argv){
    //ros节点
    std::string nodeName = "ur_test_ui";
    ros::init(args, argv, nodeName);
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle node;
    //应用程
    QApplication app(args, argv);
    MainWindow mainwindow(&node);
    mainwindow.show();
    // ros::waitForShutdown();
    return app.exec();
    // return 0;
}
