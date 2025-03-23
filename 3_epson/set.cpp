#include "set.h"
#include "ui_set.h"




Set::Set(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Set)
{
    ui->setupUi(this);
    //Drawing();
    setFixedSize(730, 255); // 设置窗口大小
    move(20,0);
    connecttogen = new ConnectToGen;
    connecttoplc = new ConnectToPlc;

    ui->stackedWidget->addWidget(connecttogen);
    ui->stackedWidget_2->addWidget(connecttoplc);
    ui->stackedWidget->setCurrentWidget(connecttogen);
    ui->stackedWidget_2->setCurrentWidget(connecttoplc);


    connect(this, SIGNAL(MidControl(int)), connecttoplc, SLOT(Control(int)));
    connect(this, &Set::Programshow, connecttoplc, &ConnectToPlc::Show);
    connect(this, &Set::Programstop, connecttoplc, &ConnectToPlc::Stop);
    connect(connecttogen, &ConnectToGen::GenControl,connecttoplc, &ConnectToPlc::Gen_Control);
    connect(connecttoplc, &ConnectToPlc::SendToGen,connecttogen, &ConnectToGen::Send_Gen);

    connect(this,&Set::CameraRead, connecttogen, &ConnectToGen::Send_Gen);

}

Set::~Set()
{
    delete ui;
}
void Set::ShowPLC()
{
    emit Programshow();
}

void Set::StopPLC()
{
    emit Programstop();
}

void Set::OperationControl(int command)
{
    emit MidControl(command);
}
void Set::CameraSet()
{
    emit CameraRead();
}

void Set::Drawing()
{

}
