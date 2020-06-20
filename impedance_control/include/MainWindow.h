//
// Created by wangneng on 6/19/20.
//

#ifndef IMPEDANCE_CONTROL_MAINWINDOW_H
#define IMPEDANCE_CONTROL_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QString>
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Bool.h"
#include "ros/ros.h"
#include <iostream>
using namespace std;

class MainWindow: public QMainWindow {
public:
    MainWindow(ros::NodeHandle *node, QWidget *parent = Q_NULLPTR);
    ~MainWindow();
public:
    ros::NodeHandle* Node;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton;
    QGroupBox *groupBox_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    void initUi(QMainWindow *MainWindow);

    ros::Publisher rbDoOnce_publisher;//机器运动一次
    ros::Subscriber ur_curPose_subscriber;//
    ros::Subscriber ur_nextPose_subscriber;//
    void callback_ur_curPose_subscriber(std_msgs::Float64MultiArray data_msg);
    void callback_ur_nextPose_subscriber(std_msgs::Float64MultiArray data_msg);

    void ur_doOnce();


};

#endif //IMPEDANCE_CONTROL_MAINWINDOW_H
