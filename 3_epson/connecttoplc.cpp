#include "connecttoplc.h"
#include "ui_connecttoplc.h"
#include <QMessageBox>
#include <QByteArray>
#include <QCoreApplication>

#define ip0 "172.16.26.23"
#define portplc 502

#define NUM_read 20
#define NUM_write 20

#define PAddress 21
#define CAddress 1

extern int StateDelt;//传送带开启状态
extern int Laser;//激光雕刻机状态
extern int Lifting;//AGV抬升状态
extern int Elimate;//剔除残次品机构
extern int StateEpson;//Epson当前操作阶段
extern int SwitchEpson;//Epson开启状态

//extern int SwitchEpson;//Epson开启状态
//extern int Sensor;//传感器状态
//extern int DetectCode;//二维码检测状态
//extern int DetectMark;//书签残次检测
//extern int Insert;//插盒机构
//extern int StateTotal;//工段当前自动状态



extern int flagcom,autocom;
extern QString com1,com2,com3,com4,com5,com6,com7,com8,com9,com10,com11,com12,com13,com14;
extern QString com15,com16,com17;


QString com;
extern int flag;

int numberc,numberr;

ConnectToPlc::ConnectToPlc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConnectToPlc)
{
    ui->setupUi(this);
    setFixedSize(400, 300); // 设置窗口大小
    move(20,200);
    this->Drawing();
    this->initWidget();

    ui->lineEditIP->setText(ip0);
    ui->lineEditPort->setText(QString::number(portplc));


//    ui->Btnsend->setEnabled(false);
//    ui->Btnclose->setEnabled(false);
//    connect(ui->Btnlisten,&QPushButton::clicked,this,&ConnectToPlc::Listened);
//    connect(ui->Btnsend,&QPushButton::clicked,this,&ConnectToPlc::SendData);
//    connect(ui->Btnclose,&QPushButton::clicked,this,&ConnectToPlc::Close);

    frameTimer1 = new QTimer(this);
//    connect(ui->Btnstop, &QPushButton::clicked, frameTimer1, &QTimer::stop);
    connect(frameTimer1, &QTimer::timeout, this, &ConnectToPlc::Receive);
//    connect(ui->Btnuse, &QPushButton::clicked, this, &ConnectToPlc::Receive);

}
ConnectToPlc::~ConnectToPlc()
{
    delete ui;
}

void ConnectToPlc::Control(int command)
{
    flag=command;
    switch(command)
    {
    case 1: com=com1;break;
    case 2: com=com2;break;
    case 3: com=com3;break;
    case 4: com=com4;break;
    case 5: com=com5;break;

    case 6: com=com6;break;
    case 7: com=com7;break;
    case 8: com=com8;break;
    case 9: com=com9;break;
    case 10: com=com10;break;

    case 11: com=com11;break;
    case 12: com=com12;break;
    case 13: com=com13;break;
    case 14: com=com14;break;

    case 15: com=com15;break;
    case 16: com=com16;break;
    case 17: com=com17;break;
    }
    Datawrite(flag,com.toInt());//flag存放位置，com存放1/0
}

void ConnectToPlc::initWidget()
{
    //初始化MyModbus对象
    m_myModsbus = new Modbus();
    connect(m_myModsbus,SIGNAL(signal_stateChanged(bool)),this,SLOT(slot_stateChanged(bool)));

    //connect(m_myModsbus,SIGNAL(signal_readCoils(QVector<quint16>)),this,SLOT(slot_readCoils(QVector<quint16>)));

    connect(m_myModsbus,SIGNAL(signal_readRegisters(int)),this,SLOT(slot_readRegisters(int)));
    connect(m_myModsbus,SIGNAL(signal_readRegisters5(int,int,int,int,int,int)),
            this,SLOT(slot_readRegisters5(int,int,int,int,int,int)));

}

void ConnectToPlc::slot_stateChanged(bool flag)
{
    if(!flag)
    {

        //ui->labelcon->setText("连接断开");
        //ui->textBack->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "连接断开");
        QMessageBox::warning(this,"警告","连接断开!");
    }
    else
    {
        //ui->labelcon->setText("连接成功");
        //ui->textBack->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "连接成功");
        QMessageBox::information(this,"promote","连接成功!");
    }
}

//void ConnectToPlc::slot_readCoils(QVector<quint16> vAllData)
//{
//    LOGDEBUG<<"readCoils size:"<<vAllData.size();
//        //LOGDEBUG<<i<<":"<<vAllData[i];

//        ui->textBack->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "读取线圈返回数据:" + QString::number(vAllData[0]));
//        ui->lineEditdataC->setText(QString::number(vAllData[0]));
//        numberc=vAllData[0];

//}

void ConnectToPlc::slot_readRegisters(int resultNum)
{
    LOGDEBUG<<"resultNum:"<<resultNum;

    //ui->textBack->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "读取寄存器返回数据:" + QString::number(resultNum));
    ui->lineEditdataR->setText(QString::number(resultNum));
    numberr=resultNum;
    Dataread(flag,numberr);
    emit SendToGen();
}

void ConnectToPlc::slot_readRegisters5(int resultNum1,int resultNum2,int resultNum3,int resultNum4,int resultNum5,int resultNum6)
{
    LOGDEBUG<<"resultNum:"<<resultNum1<<resultNum2<<resultNum3<<resultNum4<<resultNum5<<resultNum6;


    QTimer::singleShot(10, [=]() {
        numberr=resultNum1;
        Dataread(flag,numberr);
        emit SendToGen();
        flag++;
    });
    QTimer::singleShot(20, [=]() {
        numberr=resultNum2;
        Dataread(flag,numberr);
        emit SendToGen();
        flag++;
    });
    QTimer::singleShot(30, [=]() {
        numberr=resultNum3;
        Dataread(flag,numberr);
        emit SendToGen();
        flag++;
    });
    QTimer::singleShot(40, [=]() {
        numberr=resultNum4;
        Dataread(flag,numberr);
        emit SendToGen();
        flag++;
    });
    QTimer::singleShot(50, [=]() {
        numberr=resultNum5;
        Dataread(flag,numberr);
        emit SendToGen();
        flag++;
        flag++;
        flag++;
    });
    QTimer::singleShot(60, [=]() {
        numberr=resultNum6;
        Dataread(flag,numberr);
        emit SendToGen();

    });


}

void ConnectToPlc::on_Btncon_clicked()
{
    QString ip = ui->lineEditIP->text();
    int port = ui->lineEditPort->text().toInt();
    LOGDEBUG<<"ip:"<<ip<<"   port:"<<port;
    //ui->textBack->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "ip:" + ip + " port:" + QString::number(port));

    //连接到modbus设备
    m_myModsbus->connectToModbus(ip,port);
}

//void ConnectToPlc::on_BtnreadC_clicked()//unuse
//{
//    int startAdd = ui->lineEditaddC->text().toInt();
//    LOGDEBUG<<"startAdd:"<<startAdd;

//    m_myModsbus->readModbusData(1,startAdd,1);


//    Dataread(startAdd,numberc);

//}

//void ConnectToPlc::on_BtnwriteC_clicked()//unuse
//{
//    int startAdd = ui->lineEditaddC->text().toInt();
//    int writeNum = ui->lineEditdataC->text().toInt();
//    LOGDEBUG<<"startAdd:"<<startAdd<<"   writeNum:"<<writeNum;

//    //单写
//    if(!m_myModsbus->writeModbusData(1,startAdd,writeNum))
//    {
//        QMessageBox::warning(this,"警告","线圈区数据写入失败!");
//    }
//    else
//    {
//        Datawrite(startAdd,writeNum);
//    }
//}

void ConnectToPlc::on_BtnreadR_clicked()
{
    int startAdd = ui->lineEditaddR->text().toInt();
    LOGDEBUG<<"startAdd:"<<startAdd;

    m_myModsbus->readModbusData(3,startAdd,1);
    flag=startAdd;

//    Dataread(startAdd,numberr);

}

void ConnectToPlc::on_BtnwriteR_clicked()
{
    int startAdd = ui->lineEditaddR->text().toInt();
    int writeNum = ui->lineEditdataR->text().toInt();;
    LOGDEBUG<<"startAdd:"<<startAdd<<"   writeNum:"<<writeNum;
    //Datawrite(startAdd,writeNum);

    //进行写入寄存器数据的处理
    if(!m_myModsbus->writeModbusData(2,startAdd,writeNum))
    {
        QMessageBox::warning(this,"警告","寄存器区数据写入失败!");
    }
    else
    {
        Datawrite(startAdd,writeNum);
    }
}

void ConnectToPlc::Receive()
{
//    int startAdd = ui->lineEditaddR->text().toInt();
    //LOGDEBUG<<"startAdd:"<<startAdd;
    int startAdd = PAddress;

    m_myModsbus->readModbusData(3,startAdd,NUM_read);
    flag=startAdd;

}

void ConnectToPlc::Show()
{
    frameTimer1->start(70);
}

void ConnectToPlc::Stop()
{
    frameTimer1->stop();
}


//用作数据读取
void ConnectToPlc::Dataread(int startAdd,int readNum)
{
    switch (startAdd)
    {
    case 21: StateDelt=readNum;break;
    case 22: StateEpson=readNum;break;
    case 23: Laser=readNum;break;
    case 24: Lifting=readNum;break;
    case 25: Elimate=readNum;break;
    case 28: SwitchEpson=readNum;break;
    default :break;
    }
}

//用作数据写入
void ConnectToPlc::Datawrite(int flag,int writeNum)
{
    switch (flag)
    {
    case 1: m_myModsbus->writeModbusData(2,1,writeNum);break;
    case 2: m_myModsbus->writeModbusData(2,2,writeNum);break;
    case 3: m_myModsbus->writeModbusData(2,3,writeNum);break;
    case 4: m_myModsbus->writeModbusData(2,4,writeNum);break;
    case 5: m_myModsbus->writeModbusData(2,5,writeNum);break;
    case 6: m_myModsbus->writeModbusData(2,6,writeNum);break;
    case 7: m_myModsbus->writeModbusData(2,7,writeNum);break;
    case 8: m_myModsbus->writeModbusData(2,8,writeNum);break;
    case 9: m_myModsbus->writeModbusData(2,9,writeNum);break;
    case 10: m_myModsbus->writeModbusData(2,10,writeNum);break;
    case 11: m_myModsbus->writeModbusData(2,11,writeNum);break;
    case 12: m_myModsbus->writeModbusData(2,12,writeNum);break;
    case 13: m_myModsbus->writeModbusData(2,13,writeNum);break;
    case 14: m_myModsbus->writeModbusData(2,14,writeNum);break;
    case 15: m_myModsbus->writeModbusData(2,15,writeNum);break;
    case 16: m_myModsbus->writeModbusData(2,16,writeNum);break;
    case 17: m_myModsbus->writeModbusData(2,17,writeNum);break;
    default :break;
    }
}

void ConnectToPlc::Gen_Control()
{
    qDebug()<<flagcom;
    qDebug()<<autocom;
    Datawrite(flagcom,autocom);
}

void ConnectToPlc::Drawing()
{
    ui->centralwidget->setStyleSheet("background-color:rgba(170,170,220,100) ;");
    ui->Btncon->setStyleSheet("QPushButton{font: 5 10pt '微软雅黑 Light';"
                                 "color: rgb(25,25,25);"
                                 "background-color: rgba(20,20,150,100);"
                                 "border:1px solid red;"
                                 "border-color:rgb(20,20,20);"
                                 "border-radius:7px;}"
                                 "QPushButton:hover{background-color: rgba(20,100,90,100);}"//hover
                                 "QPushButton:pressed{background-color:rgba(20,30,220,100);}");//pressed

//    ui->BtnreadC->setFixedSize(62, 32);
//    ui->BtnreadC->setStyleSheet("QPushButton{font: 5 10pt '微软雅黑 Light';"
//                               "color: rgb(25,25,25);"
//                               "background-color: rgba(20,20,150,100);"
//                               "border:1px solid red;"
//                               "border-color:rgb(20,20,20);"
//                               "border-radius:7px;}"
//                               "QPushButton:hover{background-color: rgba(20,100,90,100);}"//hover
//                               "QPushButton:pressed{background-color:rgba(20,30,220,100);}");//pressed


    ui->BtnreadR->setStyleSheet("QPushButton{font: 5 10pt '微软雅黑 Light';"
                                 "color: rgb(25,25,25);"
                                 "background-color: rgba(20,20,150,100);"
                                 "border:1px solid red;"
                                 "border-color:rgb(20,20,20);"
                                 "border-radius:7px;}"
                                 "QPushButton:hover{background-color: rgba(20,100,90,100);}"//hover
                                 "QPushButton:pressed{background-color:rgba(20,30,220,100);}");//pressed


//    ui->BtnwriteC->setStyleSheet("QPushButton{font: 5 10pt '微软雅黑 Light';"
//                                "color: rgb(25,25,25);"
//                                "background-color: rgba(20,20,150,100);"
//                                "border:1px solid red;"
//                                "border-color:rgb(20,20,20);"
//                                "border-radius:7px;}"
//                                "QPushButton:hover{background-color: rgba(20,100,90,100);}"//hover
//                                "QPushButton:pressed{background-color:rgba(20,30,220,100);}");//pressed


    ui->BtnwriteR->setStyleSheet("QPushButton{font: 5 10pt '微软雅黑 Light';"
                                "color: rgb(25,25,25);"
                                "background-color: rgba(20,20,150,100);"
                                "border:1px solid red;"
                                "border-color:rgb(20,20,20);"
                                "border-radius:7px;}"
                                "QPushButton:hover{background-color: rgba(20,100,90,100);}"//hover
                                "QPushButton:pressed{background-color:rgba(20,30,220,100);}");//pressed


//    ui->textBack->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
//                                    "color: rgb(31,31,31);"		//字体颜色
//                                    "padding-left:10px;"       //内边距-字体缩进
//                                    "border:2px solid rgb(20,196,188);border-radius:15px;"
//                                    "background-color:rgba(90,240,200,100);"
//                                    "");//边框粗细-颜色-圆角设置



    ui->topiclabel->setStyleSheet("font: 40 20pt '微软雅黑 Light';"
                                    "font-weight: bold;"
                                    "border:none;"
                                    "color:rgb(40,20,20);"
                                    "background-color: transparent;");


//    ui->labelcon->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
//                                "font-weight: bold;"
//                                "border:none;"
//                                "color:rgb(40,20,20);"
//                                "background-color: transparent;");
//    ui->label_2->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
//                               "font-weight: bold;"
//                               "border:none;"
//                               "color:rgb(40,20,20);"
//                               "background-color: transparent;");
    ui->label_3->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
                               "font-weight: bold;"
                               "border:none;"
                               "color:rgb(40,20,20);"
                               "background-color: transparent;");
    ui->label_4->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
                               "font-weight: bold;"
                               "border:none;"
                               "color:rgb(40,20,20);"
                               "background-color: transparent;");
//    ui->label_5->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
//                               "font-weight: bold;"
//                               "border:none;"
//                               "color:rgb(40,20,20);"
//                               "background-color: transparent;");
//    ui->label_6->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
//                               "font-weight: bold;"
//                               "border:none;"
//                               "color:rgb(40,20,20);"
//                               "background-color: transparent;");
    ui->label_7->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
                               "font-weight: bold;"
                               "border:none;"
                               "color:rgb(40,20,20);"
                               "background-color: transparent;");
    ui->label_8->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
                               "font-weight: bold;"
                               "border:none;"
                               "color:rgb(40,20,20);"
                               "background-color: transparent;");
//    ui->label_9->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
//                               "font-weight: bold;"
//                               "border:none;"
//                               "color:rgb(40,20,20);"
//                               "background-color: transparent;");
//    ui->label_10->setStyleSheet("font: 20 10pt '微软雅黑 Light';"
//                                "font-weight: bold;"
//                                "border:none;"
//                                "color:rgb(40,20,20);"
//                                "background-color: transparent;");

    ui->lineEditPort->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
                                     "color: rgb(31,31,31);"		//字体颜色
                                     "padding-left:10px;"       //内边距-字体缩进
                                     "border:2px solid rgb(20,196,188);border-radius:15px;"
                                     "background-color:rgba(90,240,200,100);"
                                     "");//边框粗细-颜色-圆角设置

    ui->lineEditIP->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
                                     "color: rgb(31,31,31);"		//字体颜色
                                     "padding-left:10px;"       //内边距-字体缩进
                                     "border:2px solid rgb(20,196,188);border-radius:15px;"
                                     "background-color:rgba(90,240,200,100);"
                                     "");//边框粗细-颜色-圆角设置

    ui->lineEditaddR->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
                                     "color: rgb(31,31,31);"		//字体颜色
                                     "padding-left:10px;"       //内边距-字体缩进
                                     "border:2px solid rgb(20,196,188);border-radius:15px;"
                                     "background-color:rgba(90,240,200,100);"
                                     "");//边框粗细-颜色-圆角设置

    ui->lineEditdataR->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
                                     "color: rgb(31,31,31);"		//字体颜色
                                     "padding-left:10px;"       //内边距-字体缩进
                                     "border:2px solid rgb(20,196,188);border-radius:15px;"
                                     "background-color:rgba(90,240,200,100);"
                                     "");//边框粗细-颜色-圆角设置

}
