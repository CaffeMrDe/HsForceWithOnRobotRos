//
// Created by wangneng on 6/19/20.
//

#include "MainWindow.h"

MainWindow::MainWindow(ros::NodeHandle *node, QWidget *parent):QMainWindow(parent),Node(node) {
    initUi(this);
    rbDoOnce_publisher=Node->advertise<std_msgs::Bool>("/ur_workOnce", 1);
    ur_curPose_subscriber=Node->subscribe<std_msgs::Float64MultiArray>("/ur_curPose",1000,&MainWindow::callback_ur_curPose_subscriber,this);
    ur_nextPose_subscriber=Node->subscribe<std_msgs::Float64MultiArray>("/ur_nextPose",1000,&MainWindow::callback_ur_nextPose_subscriber,this);
    connect(pushButton,&QPushButton::clicked,this,&MainWindow::ur_doOnce);
}


void MainWindow::initUi(QMainWindow *MainWindow) {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(973, 727);
    centralWidget = new QWidget(MainWindow);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    verticalLayout_2 = new QVBoxLayout(centralWidget);
    verticalLayout_2->setSpacing(6);
    verticalLayout_2->setContentsMargins(11, 11, 11, 11);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    groupBox_3 = new QGroupBox(centralWidget);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
                                                "\n"
                                                "border-width:2px;\n"
                                                "\n"
                                                "border-style:solid;\n"
                                                "\n"
                                                "border-radius: 10px;\n"
                                                "\n"
                                                "border-color:gray;\n"
                                                "\n"
                                                "margin-top:0.5ex;\n"
                                                "\n"
                                                "}\n"
                                                "\n"
                                                "QGroupBox::title{\n"
                                                "\n"
                                                "subcontrol-origin:margin;\n"
                                                "\n"
                                                "subcontrol-position:top left;\n"
                                                "\n"
                                                "left:10px;\n"
                                                "\n"
                                                "margin-left:0px;\n"
                                                "\n"
                                                "padding:0 1px;\n"
                                                "\n"
                                                "}"));
    horizontalLayout = new QHBoxLayout(groupBox_3);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    verticalLayout_3 = new QVBoxLayout();
    verticalLayout_3->setSpacing(6);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label = new QLabel(groupBox_3);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout_2->addWidget(label);


    verticalLayout_3->addLayout(horizontalLayout_2);


    horizontalLayout->addLayout(verticalLayout_3);


    verticalLayout->addWidget(groupBox_3);

    groupBox = new QGroupBox(centralWidget);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
                                              "\n"
                                              "border-width:2px;\n"
                                              "\n"
                                              "border-style:solid;\n"
                                              "\n"
                                              "border-radius: 10px;\n"
                                              "\n"
                                              "border-color:gray;\n"
                                              "\n"
                                              "margin-top:0.5ex;\n"
                                              "\n"
                                              "}\n"
                                              "\n"
                                              "QGroupBox::title{\n"
                                              "\n"
                                              "subcontrol-origin:margin;\n"
                                              "\n"
                                              "subcontrol-position:top left;\n"
                                              "\n"
                                              "left:10px;\n"
                                              "\n"
                                              "margin-left:0px;\n"
                                              "\n"
                                              "padding:0 1px;\n"
                                              "\n"
                                              "}"));
    verticalLayout_5 = new QVBoxLayout(groupBox);
    verticalLayout_5->setSpacing(6);
    verticalLayout_5->setContentsMargins(11, 11, 11, 11);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    verticalLayout_4 = new QVBoxLayout();
    verticalLayout_4->setSpacing(6);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setSpacing(6);
    horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout_4->addWidget(label_2);


    verticalLayout_4->addLayout(horizontalLayout_4);

    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setSpacing(6);
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    pushButton = new QPushButton(groupBox);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    horizontalLayout_3->addWidget(pushButton);


    verticalLayout_4->addLayout(horizontalLayout_3);


    verticalLayout_5->addLayout(verticalLayout_4);


    verticalLayout->addWidget(groupBox);

    groupBox_2 = new QGroupBox(centralWidget);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
                                                "\n"
                                                "border-width:2px;\n"
                                                "\n"
                                                "border-style:solid;\n"
                                                "\n"
                                                "border-radius: 10px;\n"
                                                "\n"
                                                "border-color:gray;\n"
                                                "\n"
                                                "margin-top:0.5ex;\n"
                                                "\n"
                                                "}\n"
                                                "\n"
                                                "QGroupBox::title{\n"
                                                "\n"
                                                "subcontrol-origin:margin;\n"
                                                "\n"
                                                "subcontrol-position:top left;\n"
                                                "\n"
                                                "left:10px;\n"
                                                "\n"
                                                "margin-left:0px;\n"
                                                "\n"
                                                "padding:0 1px;\n"
                                                "\n"
                                                "}"));

    verticalLayout->addWidget(groupBox_2);


    verticalLayout_2->addLayout(verticalLayout);

    MainWindow->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(MainWindow);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 973, 31));
    MainWindow->setMenuBar(menuBar);
    mainToolBar = new QToolBar(MainWindow);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(MainWindow);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    MainWindow->setStatusBar(statusBar);
//    QMetaObject::connectSlotsByName(this);
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
    groupBox_3->setTitle(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\346\225\260\346\215\256", nullptr));
    label->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\272\344\275\215\345\247\277\346\225\260\346\215\256", nullptr));
    groupBox->setTitle(QApplication::translate("MainWindow", "\351\230\273\346\212\227\346\250\241\345\235\227\346\216\247\345\210\266", nullptr));
    label_2->setText(QApplication::translate("MainWindow", "\351\242\204\346\265\213\345\200\274", nullptr));
    pushButton->setText(QApplication::translate("MainWindow", "\346\211\247\350\241\214\344\270\200\346\254\241\350\277\220\345\212\250", nullptr));
    groupBox_2->setTitle(QApplication::translate("MainWindow", "\351\230\273\346\212\227\346\250\241\345\235\227\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));

}

MainWindow::~MainWindow() {

}

void MainWindow::callback_ur_curPose_subscriber(std_msgs::Float64MultiArray data_msg) {

    label->setText(QString("[%1,%2,%3,%4,%5,%6]").arg(data_msg.data[0]).arg(data_msg.data[1]).arg(data_msg.data[2]).arg(data_msg.data[3]).arg(data_msg.data[4]).arg(data_msg.data[5]));
}

void MainWindow::callback_ur_nextPose_subscriber(std_msgs::Float64MultiArray data_msg) {
    label_2->setText(QString("%1,%2,%3,%4,%5,%6").arg(data_msg.data[0]).arg(data_msg.data[1]).arg(data_msg.data[2]).arg(data_msg.data[3]).arg(data_msg.data[4]).arg(data_msg.data[5]));
}

void MainWindow::ur_doOnce() {
    std_msgs::Bool msg;
    msg.data= true;
    rbDoOnce_publisher.publish(msg);
    cout<<"点击了按钮"<<endl;
}


