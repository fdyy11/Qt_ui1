#include "mainwindow.h"
#include "ui_mainwindow.h"


//数据显示界面变量
int StateDelt=0;//传送带开启状态
int SwitchEpson=0;//Epson开启状态
int Sensor=0;//传感器状态
int DetectMark=0;//书签残次检测
int DetectCode=0;//二维码检测信息
int Laser=0;//激光雕刻机状态
int Lifting=0;//AGV抬升状态
int Elimate=0;//剔除残次品机构
int Insert=0;//插盒机构
int StateTotal=0;//工段当前自动状态

int StateEpson=0;//Epson当前操作阶段

int flagcom=0,autocom=0;
//控制界面变量
QString com1,com2,com3,com4,com5,com6,com7,com8,com9,com10,com11,com12,com13,com14;
QString com15,com16,com17;

int flag;
//视觉信息处理
extern int E_box,E_detect;

//启停按钮状态控制
int st=0,em=0;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(891, 700); // 设置窗口大小
    move(20,0);
    this->setWindowTitle("数据显示界面");
    Drawing();
//    connecttogen = new ConnectToGen;
//    connecttoplc = new ConnectToPlc;
    program = new Program;
    operation = new Operation;
    set=new Set;
    TimerReset = new QTimer;


    ui->stackedWidget->addWidget(program);
    ui->stackedWidget->addWidget(operation);
    ui->stackedWidget->setCurrentWidget(program);


    connect(ui->Btnmain, &QPushButton::clicked, this, [&](){
        // 显示数据窗口
        ui->stackedWidget->setCurrentWidget(program);
        ui->centralwidget->setStyleSheet("background-color: rgb(0,0,185);");
        this->setWindowTitle("数据显示界面");
    });
    connect(ui->Btnope, &QPushButton::clicked, this, [&](){
        // 显示操作窗口
        ui->stackedWidget->setCurrentWidget(operation);
        ui->centralwidget->setStyleSheet("background-color: rgb(255,114,86);");
        this->setWindowTitle("操作界面");
    });


    connect(operation, SIGNAL(Btnclicked(int)), set, SLOT(OperationControl(int)));
    connect(this, SIGNAL(Start(int)), set, SLOT(OperationControl(int)));
    connect(this, SIGNAL(Stop(int)), set, SLOT(OperationControl(int)));
    connect(this, SIGNAL(Emergy1(int)), set, SLOT(OperationControl(int)));
    connect(this, SIGNAL(Emergy2(int)), set, SLOT(OperationControl(int)));
    connect(this, SIGNAL(Reset(int)), set, SLOT(OperationControl(int)));

    connect(program, &Program::CameraRead, set, &Set::CameraSet);
    connect(program, &Program::StartShow, set, &Set::ShowPLC);
    connect(program, &Program::StopShow, set, &Set::StopPLC);

    connect(program,&Program::ConClicked, this, [&](){
        // 显示设置窗口
        set->setWindowTitle("连接设置界面");
        set->show();
    });

    connect(ui->Btnstart, &QPushButton::clicked, this, [&](){
        if(st == 0&& em == 0)
        {
            ui->Btnstart->setStyleSheet("QPushButton {"
                                     "font: 25 20pt '微软雅黑 Light';"
                                     "color: rgb(255,255,255);"
                                     "background-color: rgb(130,240,0); "
                                     "border: none; border-radius: 15px;"
                                     "}"
                                     "QPushButton:hover {"
                                     "background-color: rgb(100,210,0);"
                                     "}"
                                     "QPushButton:pressed {"
                                     "background-color: rgb(160,255,30);"
                                     "}");

            ui->Btnstop->setStyleSheet("QPushButton {"
                                       "font: 25 20pt '微软雅黑 Light';"
                                       "color: rgb(255,255,255);"
                                       "background-color: rgb(200,200,200); "
                                       "border: none; border-radius: 15px;"
                                       "}"
                                       "QPushButton:hover {"
                                       "background-color: rgb(170,170,170);"
                                       "}"
                                       "QPushButton:pressed {"
                                       "background-color: rgb(230,230,230);"
                                       "}");

            st = !st;


        }
        com15="1";
        emit Start(15);
    });
    connect(ui->Btnstop, &QPushButton::clicked, this, [&](){
        if(st != 0&& em == 0)
        {
            ui->Btnstop->setStyleSheet("QPushButton {"
                                       "font: 25 20pt '微软雅黑 Light';"
                                       "color: rgb(255,255,255);"
                                       "background-color: rgb(220,20,60); "
                                       "border: none; border-radius: 15px;"
                                       "}"
                                       "QPushButton:hover {"
                                       "background-color: rgb(180,0,0);"
                                       "}"
                                       "QPushButton:pressed {"
                                       "background-color: rgb(250,60,60);"
                                       "}");

            ui->Btnstart->setStyleSheet("QPushButton {"
                                        "font: 25 20pt '微软雅黑 Light';"
                                        "color: rgb(255,255,255);"
                                        "background-color: rgb(200,200,200); "
                                        "border: none; border-radius: 15px;"
                                        "}"
                                        "QPushButton:hover {"
                                        "background-color: rgb(170,170,170);"
                                        "}"
                                        "QPushButton:pressed {"
                                        "background-color: rgb(230,230,230);"
                                        "}");

            st = !st;


        }
        com15="0";
        emit Stop(15);
    });

    connect(ui->Btnemergy, &QPushButton::clicked, this, [&](){
        if(em==0)
        {
            ui->Btnemergy->setStyleSheet("QPushButton {"
                                         "font: 25 20pt '微软雅黑 Light';"
                                         "color: rgb(255,255,255);"
                                         "background-color: rgb(250,0,0); "
                                         "border: none; border-radius: 15px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "background-color: rgb(220,0,0);"
                                         "}"
                                         "QPushButton:pressed {"
                                         "background-color: rgb(255,30,30);"
                                         "}");

            ui->Btnstop->setStyleSheet("QPushButton {"
                                       "font: 25 20pt '微软雅黑 Light';"
                                       "color: rgb(255,255,255);"
                                       "background-color: rgb(200,200,200); "
                                       "border: none; border-radius: 15px;"
                                       "}"
                                       "QPushButton:hover {"
                                       "background-color: rgb(170,170,170);"
                                       "}"
                                       "QPushButton:pressed {"
                                       "background-color: rgb(230,230,230);"
                                       "}");

            ui->Btnstart->setStyleSheet("QPushButton {"
                                        "font: 25 20pt '微软雅黑 Light';"
                                        "color: rgb(255,255,255);"
                                        "background-color: rgb(200,200,200); "
                                        "border: none; border-radius: 15px;"
                                        "}"
                                        "QPushButton:hover {"
                                        "background-color: rgb(170,170,170);"
                                        "}"
                                        "QPushButton:pressed {"
                                        "background-color: rgb(230,230,230);"
                                        "}");
            st=0;
            com15="0";
            emit Stop(15);
            com16="1";
            emit Emergy1(16);
            em=1;
//            this->setWindowIcon(QIcon(":icons/1.ico"));

        }


         });

    connect(TimerReset,&QTimer::timeout,this,&MainWindow::TimeReset);
    connect(ui->Btnreset, &QPushButton::clicked, this, [&](){
        if(em!=0)
        {
            ui->Btnreset->setStyleSheet("QPushButton {"
                                        "font: 25 20pt '微软雅黑 Light';"
                                        "color: rgb(255,255,255);"
                                        "background-color: rgb(0,240,0); "
                                        "border: none; border-radius: 15px;"
                                        "}"
                                        "QPushButton:hover {"
                                        "background-color: rgb(0,210,0);"
                                        "}"
                                        "QPushButton:pressed {"
                                        "background-color: rgb(30,255,30);"
                                        "}");
            ui->Btnemergy->setStyleSheet("QPushButton {"
                                         "font: 25 20pt '微软雅黑 Light';"
                                         "color: rgb(255,255,255);"
                                         "background-color: rgb(200,200,200); "
                                         "border: none; border-radius: 15px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "background-color: rgb(170,170,170);"
                                         "}"
                                         "QPushButton:pressed {"
                                         "background-color: rgb(230,230,230);"
                                         "}");
            em=0;
            com17="1";
            emit Reset(17);
            com16="0";
            emit Emergy1(16);

            TimerReset->start(1000);
        }

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TimeReset()
{
    ui->Btnreset->setStyleSheet("QPushButton {"
                                "font: 25 20pt '微软雅黑 Light';"
                                "color: rgb(255,255,255);"
                                "background-color: rgb(200,200,200); "
                                "border: none; border-radius: 15px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: rgb(170,170,170);"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgb(230,230,230);"
                                "}");
    ui->Btnstop->setStyleSheet("QPushButton {"
                               "font: 25 20pt '微软雅黑 Light';"
                               "color: rgb(255,255,255);"
                               "background-color: rgb(220,20,60); "
                               "border: none; border-radius: 15px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: rgb(180,0,0);"
                               "}"
                               "QPushButton:pressed {"
                               "background-color: rgb(250,60,60);"
                               "}");
    com17="0";
    emit Reset(17);
    com15="0";
    emit Stop(15);
    TimerReset->stop();

}



//ui界面设计
void MainWindow::Drawing()
{

    ui->centralwidget->setStyleSheet("background-color: rgb(0,0,185);");
//    ui->centralwidget->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(250,167,85, 60),stop:1 rgba(252,100,40, 150));");
//    ui->Btngen->setFixedSize(120, 72);
//    ui->Btngen->setStyleSheet("QPushButton{font: 25 20pt '微软雅黑 Light';"
//                              "color: rgb(255,255,255);"
//                              "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:1 rgba(252,175,123,100),stop:0 rgba(241,90,34, 210));"
//                              "border: none;border-radius:15px;}"
//                              "QPushButton:hover{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(232,160,110, 100), stop:1 rgba(221,75,19, 210));}"//hover
//                              "QPushButton:pressed{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(255,190,140, 100), stop:1 rgba(251,105,49, 210));}");//pressed

//    ui->Btnplc->setFixedSize(120, 72);
//    ui->Btnplc->setStyleSheet("QPushButton{font: 25 20pt '微软雅黑 Light';"
//                              "color: rgb(255,255,255);"
//                              "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:1 rgba(252,175,123,100),stop:0 rgba(241,90,34, 210));"
//                              "border: none;border-radius:15px;}"
//                              "QPushButton:hover{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(232,160,110, 100), stop:1 rgba(221,75,19, 210));}"//hover
//                              "QPushButton:pressed{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(255,190,140, 100), stop:1 rgba(251,105,49, 210));}");//pressed


    ui->Btnmain->setFixedSize(120, 72);
    ui->Btnmain->setStyleSheet("QPushButton{font: 25 20pt '微软雅黑 Light';"
                               "color: rgb(255,255,255);"
                               "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:1 rgb(252,175,123),stop:0 rgb(241,90,34));"
                               "border: none;border-radius:15px;}"
                               "QPushButton:hover{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgb(232,160,110), stop:1 rgb(221,75,19));}"//hover
                               "QPushButton:pressed{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgb(255,190,140), stop:1 rgb(251,105,49));}");//pressed

    ui->Btnope->setFixedSize(120, 72);
    ui->Btnope->setStyleSheet("QPushButton{font: 25 20pt '微软雅黑 Light';"
                              "color: rgb(255,255,255);"
                              "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:1 rgb(84,255,159),stop:0 rgb(84,139,84));"
                              "border: none;border-radius:15px;}"
                              "QPushButton:hover{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgb(64,235,140), stop:1 rgb(64,119,64));}"//hover
                              "QPushButton:pressed{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgb(104,255,180), stop:1 rgb(104,159,104));}");//pressed


    ui->Btnstart->setFixedSize(120, 72);
    ui->Btnstart->setStyleSheet("QPushButton {"
                                "font: 25 20pt '微软雅黑 Light';"
                                "color: rgb(255,255,255);"
                                "background-color: rgb(200,200,200); "
                                "border: none; border-radius: 15px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: rgb(170,170,170);"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgb(240,240,240);"
                                "}");
    ui->Btnstop->setFixedSize(120, 72);
    ui->Btnstop->setStyleSheet("QPushButton {"
                                "font: 25 20pt '微软雅黑 Light';"
                                "color: rgb(255,255,255);"
                                "background-color: rgb(220,20,60); "
                                "border: none; border-radius: 15px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: rgb(180,0,0);"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgb(250,60,60);"
                                "}");

    ui->Btnemergy->setFixedSize(120, 72);
    ui->Btnemergy->setStyleSheet("QPushButton {"
                                 "font: 25 20pt '微软雅黑 Light';"
                                 "color: rgb(255,255,255);"
                                 "background-color: rgb(200,200,200); "
                                 "border: none; border-radius: 15px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color: rgb(170,170,170);"
                                 "}"
                                 "QPushButton:pressed {"
                                 "background-color: rgb(230,230,230);"
                                 "}");
    ui->Btnreset->setFixedSize(120, 72);
    ui->Btnreset->setStyleSheet("QPushButton {"
                                "font: 25 20pt '微软雅黑 Light';"
                                "color: rgb(255,255,255);"
                                "background-color: rgb(200,200,200); "
                                "border: none; border-radius: 15px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: rgb(170,170,170);"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgb(230,230,230);"
                                "}");


}
