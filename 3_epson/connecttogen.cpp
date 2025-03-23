#include "connecttogen.h"
#include "ui_connecttogen.h"
#include <QMessageBox>

#define ip0 "192.168.226.1"
#define port1 3300
#define port2 3400

extern int StateDelt;//传送带开启状态
extern int SwitchEpson;//Epson开启状态
extern int DetectCode;//二维码检测状态
extern int DetectMark;//书签残次检测
extern int Sensor;//传感器状态
extern int Laser;//激光雕刻机状态
extern int Lifting;//AGV抬升状态
extern int Elimate;//剔除残次品机构
extern int Insert;//插盒机构
extern int StateTotal;//工段当前自动状态

extern int StateEpson;//Epson当前操作阶段

extern int flag,numberr;
extern int flagcom,autocom;
extern QString com1,com2,com3,com4,com5,com6,com7,com8,com9,com10,com11,com12,com13,com14;

std::string ToGenData;

ConnectToGen::ConnectToGen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConnectToGen)
{
    ui->setupUi(this);
    Drawing();
    setFixedSize(350, 300); // 设置窗口大小
    move(20,200);

    ui->lineEditIP->setText(ip0);//获取服务器的ip地址
    ui->lineEditPort1->setText(QString::number(port1));//获取服务器的端口号
    ui->lineEditPort2->setText(QString::number(port2));//获取服务器的端口号



    ui->Btnsend->setEnabled(false);
    this->tcpClient1 = new QTcpSocket(this);//读取上位机，上传下
    this->tcpClient2 = new QTcpSocket(this);//写给上位机，下传上

    connect( this->tcpClient1, &QTcpSocket::connected, this, &ConnectToGen::TCPClient1);
    connect( this->tcpClient2, &QTcpSocket::connected, this, &ConnectToGen::TCPClient2);

    connect(this->tcpClient1, &QTcpSocket::readyRead,this, &ConnectToGen::ReadInformation);

    connect(ui->Btncon,&QPushButton::clicked,this,&ConnectToGen::Connected);
    connect(ui->Btnsend,&QPushButton::clicked,this,&ConnectToGen::SendData);




    //    frameTimer1 = new QTimer(this);
    //    connect(frameTimer1, &QTimer::timeout, this, &ConnectToGen::ReadInformation);
    //    frameTimer1->start(15); // 20帧/秒（1000毫秒/20帧）
}

ConnectToGen::~ConnectToGen()
{
    delete ui;
}

void ConnectToGen::TCPClient1()
{
    ui->textEditMoni->append("已成功连接，准备写入");
}

void ConnectToGen::TCPClient2()
{
    ui->textEditMoni->append("已成功连接，准备读取");
}
void ConnectToGen::ReadInformation()
{
    //获取套接字中的内容
    QByteArray temp =tcpClient1->readAll();
    std::vector<uchar> data(temp.begin(), temp.end());
    decodeMessage(data);
    emit GenControl();
    //qDebug()<<data;
//    if(!temp.isEmpty())
//        ui->textEditRecv->append(temp);
}

void ConnectToGen::Connected()
{
    QString IP1 = ui->lineEditIP->text();
    qint16 PORT1 = ui->lineEditPort1->text().toInt();//获取服务器的端口号
    qint16 PORT2 = ui->lineEditPort2->text().toInt();//获取服务器的端口号
    if(tcpClient1->state() == QAbstractSocket::UnconnectedState && tcpClient2->state() == QAbstractSocket::UnconnectedState)//第二步再判断tcpClient套接字的状态是否建立连接，这里判断是没有建立连接
    {
        //主动和服务器进行连接
        tcpClient1->connectToHost(QHostAddress(IP1),PORT1);
        tcpClient2->connectToHost(QHostAddress(IP1),PORT2);
        //连接成功进入if语句
        if(tcpClient1->waitForConnected(1000))
        {
            ui->Btncon->setText("断开");
            ui->Btnsend->setEnabled(true);
        }
    }
    else//这里判断建立连接的情况
    {
        tcpClient1->disconnectFromHost();
        tcpClient2->disconnectFromHost();
        //已断开进入if语句
        if(tcpClient1->state() == QAbstractSocket::UnconnectedState||\
                                                                      tcpClient1->waitForDisconnected(1000))
        {
            ui->Btncon->setText("连接");
            ui->Btnsend->setEnabled(false);
        }
    }
}


void ConnectToGen::SendData()
{
    QString str= ui->textEditMoni->toPlainText();
    int ret= this->tcpClient2->write(str.toUtf8().data());
    if(ret==-1)
    {
        //QMessageBox::information(this,"promote","发送失败");
    }
    else
    {
        //QMessageBox::information(this,"promote","发送成功");
    }
}

void ConnectToGen::Send_Gen()
{
    encodeMessage();
    QByteArray bytedata(ToGenData.c_str(), ToGenData.length());
    int ret= this->tcpClient2->write(bytedata);
    if(ret==-1)
    {
        //QMessageBox::information(this,"promote","发送失败");
    }
    else
    {
       //QMessageBox::information(this,"promote","发送成功");
    }
}

void ConnectToGen::encodeMessage()//编码
{
    std::string frameHeader = "AAAA";
    std::string frameFooter = "FFFF";

    std::string flagControl = "E03";

    //电控数据标志位
    std::string flag1 = "P01";
    std::string flag2 = "P02";
    std::string flag3 = "P03";
    std::string flag4 = "P04";
    std::string flag5 = "P05";
    //视觉数据标志位
    std::string flag6 = "P06";
    std::string flag7 = "P07";
    std::string flag8 = "P08";


    std::string encodedData;

    // 构建编码数据
    encodedData += frameHeader;

    switch(flag)
    {
    case 21: encodedData += flagControl + flag1 + std::to_string(numberr);break;
    case 22: encodedData += flagControl + flag2 + std::to_string(numberr);break;
    case 23: encodedData += flagControl + flag3 + std::to_string(numberr);break;
    case 24: encodedData += flagControl + flag4 + std::to_string(numberr);break;
    case 25: encodedData += flagControl + flag5 + std::to_string(numberr);break;

    case 26: encodedData += flagControl + flag6 + std::to_string(numberr);break;
        //qDebug()<<encodedData;
    case 27: encodedData += flagControl + flag7 + std::to_string(numberr);break;

    case 28: encodedData += flagControl + flag8 + std::to_string(numberr);break;


    }

    encodedData += frameFooter;
    ToGenData=encodedData;


}

//解码文本数据
void ConnectToGen::decodeMessage(const std::vector<uchar>& encodedData) {
    std::string frameHeader = "AAAA";
    std::string frameFooter = "FFFF";

    std::string flagControl = "E03";


    std::string flag1 = "C01";
    std::string flag2 = "C02";
    std::string flag3 = "C03";
    std::string flag4 = "C04";
    std::string flag5 = "C05";
    std::string flag6 = "C06";
    std::string flag7 = "C07";
    std::string flag8 = "C08";
    std::string flag9 = "C09";
    std::string flag10 = "C10";
    std::string flag11 = "C11";
    std::string flag12 = "C12";
    std::string flag13 = "C13";
    std::string flag14 = "C14";
    std::string flag15 = "C15";
    std::string flag16 = "C16";
    std::string flag17 = "C17";



    std::string strEncodedData(encodedData.begin(), encodedData.end());

    size_t headerPos = strEncodedData.find(frameHeader);
    if (headerPos != std::string::npos) {
        size_t flag1Pos = headerPos + frameHeader.size();
        std::string decodedMessage;

        size_t signPos = strEncodedData.find(flagControl);
        if (flag1Pos != std::string::npos) {
            size_t flag2Pos = signPos + flagControl.size();

            //1
            if (strEncodedData.compare(flag2Pos, flag1.size(), flag1) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag1.size(), footerPos - (flag2Pos + flag1.size()));
                    qDebug()<< "传送带开关:" << decodedMessage << Qt::endl;
                    //赋值
                    flagcom=1;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }
            //2
            else if (strEncodedData.compare(flag2Pos, flag2.size(), flag2) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag2.size(), footerPos - (flag2Pos + flag2.size()));
                    qDebug()<< "Epson开关:" << decodedMessage << Qt::endl;
                    //赋值
                    flagcom=2;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }
            //3
            else if (strEncodedData.compare(flag2Pos, flag3.size(), flag3) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag3.size(), footerPos - (flag2Pos + flag3.size()));
                    qDebug()<< "激光雕刻开关:" << decodedMessage << Qt::endl;
                    //赋值
                    flagcom=3;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

            //4
            else if (strEncodedData.compare(flag2Pos, flag4.size(), flag4) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag4.size(), footerPos - (flag2Pos + flag4.size()));
                    qDebug()<< "Agv抬升开关:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=4;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }
            //5
            else if (strEncodedData.compare(flag2Pos, flag5.size(), flag5) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag5.size(), footerPos - (flag2Pos + flag5.size()));
                    qDebug()<< "插盒机构气缸1:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=5;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

            //6
            else if (strEncodedData.compare(flag2Pos, flag6.size(), flag6) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag6.size(), footerPos - (flag2Pos + flag6.size()));
                    qDebug()<< "插盒机构气缸2:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=6;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }
            //7
            else if (strEncodedData.compare(flag2Pos, flag7.size(), flag7) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag7.size(), footerPos - (flag2Pos + flag7.size()));
                    qDebug()<< "插盒机构气缸3:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=7;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

            //8
            else if (strEncodedData.compare(flag2Pos, flag8.size(), flag8) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag8.size(), footerPos - (flag2Pos + flag8.size()));
                    qDebug()<< "吸盘开关:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=8;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }
            //9
            else if (strEncodedData.compare(flag2Pos, flag9.size(), flag9) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag9.size(), footerPos - (flag2Pos + flag9.size()));
                    qDebug()<< "夹爪开关:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=9;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

            //10
            else if (strEncodedData.compare(flag2Pos, flag10.size(), flag10) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag10.size(), footerPos - (flag2Pos + flag10.size()));
                    qDebug()<< "剔除开关:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=10;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }
            //11
            else if (strEncodedData.compare(flag2Pos, flag11.size(), flag11) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag11.size(), footerPos - (flag2Pos + flag11.size()));
                    qDebug()<< "伺服1:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=11;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

            //12
            else if (strEncodedData.compare(flag2Pos, flag12.size(), flag12) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag12.size(), footerPos - (flag2Pos + flag12.size()));
                    qDebug()<< "伺服2:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=12;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }
            //13
            else if (strEncodedData.compare(flag2Pos, flag13.size(), flag13) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag13.size(), footerPos - (flag2Pos + flag13.size()));
                    qDebug()<< "伺服气缸:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=13;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

            //14
            else if (strEncodedData.compare(flag2Pos, flag14.size(), flag14) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag14.size(), footerPos - (flag2Pos + flag14.size()));
                    qDebug()<< "Epson 状态控制:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=14;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

            //15
            else if (strEncodedData.compare(flag2Pos, flag15.size(), flag15) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag15.size(), footerPos - (flag2Pos + flag15.size()));
                    qDebug()<< "自动状态:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=15;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

            //16
            else if (strEncodedData.compare(flag2Pos, flag16.size(), flag16) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag16.size(), footerPos - (flag2Pos + flag16.size()));
                    qDebug()<< "急停:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=16;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

            //17
            else if (strEncodedData.compare(flag2Pos, flag17.size(), flag17) == 0) {
                size_t footerPos = strEncodedData.find(frameFooter, flag2Pos);
                if (footerPos != std::string::npos) {
                    decodedMessage = strEncodedData.substr(flag2Pos + flag17.size(), footerPos - (flag2Pos + flag17.size()));
                    qDebug()<< "复位:" << decodedMessage << Qt::endl;
                            //赋值
                            flagcom=17;
                    autocom= std::stof(decodedMessage);
                }
                else {
                    qDebug() << "未找到帧尾!"<< Qt::endl ;
                }
            }

        }

        else {
            qDebug() << "未找到标志位!" << Qt::endl;
        }
    }
    else {
        qDebug()<< "未找到帧头!"<< Qt::endl ;
    }
}


void ConnectToGen::Drawing()
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
    ui->Btnsend->setStyleSheet("QPushButton{font: 5 10pt '微软雅黑 Light';"
                              "color: rgb(25,25,25);"
                              "background-color: rgba(20,20,150,100);"
                              "border:1px solid red;"
                              "border-color:rgb(20,20,20);"
                              "border-radius:7px;}"
                              "QPushButton:hover{background-color: rgba(20,100,90,100);}"//hover
                              "QPushButton:pressed{background-color:rgba(20,30,220,100);}");//pressed
    ui->textEditMoni->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色
                                    "padding-left:10px;"       //内边距-字体缩进
                                    "border:2px solid rgb(20,196,188);border-radius:15px;"
                                    "background-color:rgba(90,240,200,100);"
                                    "");//边框粗细-颜色-圆角设置
    ui->textEditMoni->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->textEditMoni->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//    ui->textEditSend->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
//                                    "color: rgb(31,31,31);"		//字体颜色
//                                    "padding-left:10px;"       //内边距-字体缩进
//                                    "border:2px solid rgb(20,196,188);border-radius:15px;"
//                                    "background-color:rgba(90,240,200,100);"
//                                    "");//边框粗细-颜色-圆角设置

//    ui->topicLabel->setStyleSheet("font: 40 40pt '微软雅黑 Light';"
//                                  "font-weight: bold;"
//                                  "border:none;"
//                                  "color:rgb(255,255,255);"
//                                  "background-color: transparent;");

    ui->topicLabel->setStyleSheet("font: 40 20pt '微软雅黑 Light';"
                                  "font-weight: bold;"
                                  "border:none;"
                                  "color:rgb(40,20,20);"
                                  "background-color: transparent;");

    ui->label_2->setStyleSheet("font: 40 10pt '微软雅黑 Light';"
                                  "font-weight: bold;"
                                  "border:none;"
                                  "color:rgb(10,10,10);"
                                  "background-color: transparent;");
    ui->label_3->setStyleSheet("font: 40 10pt '微软雅黑 Light';"
                                  "font-weight: bold;"
                                  "border:none;"
                                  "color:rgb(10,10,10);"
                                  "background-color: transparent;");
    ui->label_4->setStyleSheet("font: 40 20pt '微软雅黑 Light';"
                                  "font-weight: bold;"
                                  "border:none;"
                                  "color:rgb(10,10,10);"
                                  "background-color: transparent;");
//    ui->label_5->setStyleSheet("font: 40 20pt '微软雅黑 Light';"
//                                  "font-weight: bold;"
//                                  "border:none;"
//                                  "color:rgb(10,10,10);"
//                                  "background-color: transparent;");

//    ui->Btncon->setStyleSheet("QPushButton{font: 25 16pt '微软雅黑 Light';"
//                                  "color: rgb(255,255,255);"
//                                  "background-color: rgba(50,50,255,90);"
//                                  "border: none;"
//                                  "border-radius:7px;}"
//                                  "QPushButton:hover{background-color:rgba(100,100,255,70) ;}"
//                                  "QPushButton:pressed{background-color:rgba(0,0,255,110);}");
//    ui->Btnsend->setStyleSheet("QPushButton{font: 25 16pt '微软雅黑 Light';"
//                                   "color: rgb(255,255,255);"
//                                   "background-color: rgba(50,50,225,90);"
//                                   "border: none;"
//                                   "border-radius:7px;}"
//                                   "QPushButton:hover{background-color:rgba(100,100,195,70) ;}"
//                                   "QPushButton:pressed{background-color:rgba(0,0,255,110);}");

    ui->lineEditIP->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
                                  "color: rgb(31,31,31);"		//字体颜色
                                  "padding-left:10px;"       //内边距-字体缩进
                                  "border:2px solid rgb(20,196,188);border-radius:15px;"
                                  "background-color:rgba(90,240,200,100);"
                                  "");//边框粗细-颜色-圆角设置

    ui->lineEditPort1->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
                                  "color: rgb(31,31,31);"		//字体颜色
                                  "padding-left:10px;"       //内边距-字体缩进
                                  "border:2px solid rgb(20,196,188);border-radius:15px;"
                                  "background-color:rgba(90,240,200,100);"
                                  "");//边框粗细-颜色-圆角设置



    ui->lineEditPort2->setStyleSheet("font: 32 10pt '微软雅黑 Light';" //字体
                                     "color: rgb(31,31,31);"		//字体颜色
                                     "padding-left:10px;"       //内边距-字体缩进
                                     "border:2px solid rgb(20,196,188);border-radius:15px;"
                                     "background-color:rgba(90,240,200,100);"
                                     "");//边框粗细-颜色-圆角设置

}
