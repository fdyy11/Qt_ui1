#include "program.h"
#include "ui_program.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QPixmap>


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

int detectnum=0;

int cnt=1;

extern int E_box,E_detect;

Program::Program(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Program)
{
    ui->setupUi(this);
    setFixedSize(891, 641); // 设置窗口大小
    move(20,0);
    camera=new Camera;
    Drawing();
    openDatabases();

    ui->stackedWidget->addWidget(camera);
    ui->stackedWidget->setCurrentWidget(camera);
    ui->stackedWidget->setFixedSize(480, 520);

    QPixmap pixmap("D:/QT projects/3_epson/icons/epson.png");
    QSize newSize(244, 231);  // 新的大小，这里是200x200
    QPixmap pixmap1 = pixmap.scaled(newSize, Qt::KeepAspectRatio);
    ui->labelpicture->setPixmap(pixmap1);


    connect(camera, &Camera::CameraData, this, &Program::CameraEncode);

    frameTimer1 = new QTimer(this);
    connect(frameTimer1, &QTimer::timeout, this, &Program::Update);

    connect(ui->Btnenter,&QPushButton::clicked,this,&Program::EnterDatabase);
//    connect(ui->Btnshow,&QPushButton::clicked,this,&Program::EnterDatabase);
    frameTimer1->start(100); // 20帧/秒（1000毫秒/20帧）
}

Program::~Program()
{
    delete ui;
}

void Program::on_Btnshow_clicked()
{
    emit StartShow();
}

void Program::on_Btnstop_clicked()
{
    emit StopShow();
}

void Program::CameraEncode()
{
    emit CameraRead();
}

void Program::on_Btncon_clicked()
{
    emit ConClicked();
}

void Program::openDatabases(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("mysql"); //不是Database,记得别填错
    db.setUserName("root");
    db.setPassword("111111");
    db.open();
    if (db.open()) {
        //QMessageBox::information(this, "Information", "Database connected successfully");
        qDebug()<<"open database successfully";

    }
    else {
        //QMessageBox::information(this, "infor", "link failed");
        qDebug()<<"error open database because"<<db.lastError().text();
    }
}

void Program::EnterDatabase()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString usetime = currentTime.toString("hh:mm:ss");

    QSqlQuery updateQuery0;
    updateQuery0.prepare("UPDATE epson_user SET StateDelt = :SwD, "
                         "StateEpson = :StE,"
                         "NumberCode = :Nb, DetectMark = :Ed, "
                         " Laser = :La, "
                         "Lifting = :Li, Elimate = :El, "
                         "UseTime = :UT "
                         "WHERE ID = :sno");
    updateQuery0.bindValue(":SwD", StateDelt);
    updateQuery0.bindValue(":StE", StateEpson);
    updateQuery0.bindValue(":Nb", E_box);
    updateQuery0.bindValue(":Ed", E_detect);
    updateQuery0.bindValue(":La", Laser);
    updateQuery0.bindValue(":Li", Lifting);
    updateQuery0.bindValue(":El", Elimate);

    updateQuery0.bindValue(":UT", usetime);

    updateQuery0.bindValue(":sno", cnt);

    if (updateQuery0.exec()) {
        // 更新数据库成功
        qDebug() << "Data updated in the database for Sno " << cnt;
        //QMessageBox::information(this, "Success", "Data updated in the database for Sno " + QString::number(currentSno));
    } else {
        // 更新数据库失败，处理错误
        QString errorMessage = updateQuery0.lastError().text();
        qDebug() << "Error updating database: " << errorMessage;
        QMessageBox::critical(this, "Error", "Failed to update database. Error: " + errorMessage);
    }
    cnt++;

}

void Program::Update(){
    if(StateDelt)
    {
        ui->labeldelt->setText("已开启");
//        setLED(ui->labeldelt, 2, 20);
    }
    else
    {
        ui->labeldelt->setText("关闭");
//        setLED(ui->labeldelt, 1, 20);
    }


    if(Laser)
    {
        ui->labellas->setText("运行中");
//        setLED(ui->labellas, 2, 20);
    }
    else
    {
        ui->labellas->setText("关闭");
//        setLED(ui->labellas, 1, 20);
    }

    if(E_detect)
    {
        ui->labeldetmark->setText(QString("%1号 残次").arg(E_box));
//        setLED(ui->labeldetmark, 2, 20);
    }
    else
    {
        ui->labeldetmark->setText(QString("%1号 正常").arg(E_box));
//        setLED(ui->labeldetmark, 1, 20);
    }
    if(Elimate)
    {
        ui->labeleli->setText("开启");
//        setLED(ui->labeleli, 2, 20);
    }
    else
    {
        ui->labeleli->setText("关闭");
//        setLED(ui->labeleli, 1, 20);
    }

    if(Lifting)
    {
        ui->labellift->setText("抬升中");
//        setLED(ui->labellift, 2, 20);
    }
    else
    {
        ui->labellift->setText("关闭");
//        setLED(ui->labellift, 1, 20);
    }


    if(SwitchEpson==0)
    {
        ui->labelepson->setText("未开启");
    }
    else if(StateEpson==1 || StateEpson==0)
    {
        ui->labelepson->setText("第1列装配");
    }
    else if(StateEpson==2)
    {
        ui->labelepson->setText("第2列装配");
    }
    else if(StateEpson==3)
    {
        ui->labelepson->setText("第3列装配");
    }
    else if(StateEpson==4)
    {
        ui->labelepson->setText("第4列装配");
    }
    else if(StateEpson==5)
    {
        ui->labelepson->setText("第5列装配");
    }



}

void Program::setLED(QLabel* label, int color, int size)
{
//    // 将label中的文字清空
//    label->setText("");
//    // 先设置矩形大小
//    // 如果ui界面设置的label大小比最小宽度和高度小，矩形将被设置为最小宽度和最小高度；
//    // 如果ui界面设置的label大小比最小宽度和高度大，矩形将被设置为最大宽度和最大高度；
//    QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
//    QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
//    QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
//    QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
//    // 再设置边界形状及边框
//    QString border_radius = QString("border-radius: %1px;").arg(size/2);    // 边框是圆角，半径为size/2
//    QString border = QString("border:1px solid black;");                    // 边框为1px黑色
//    // 最后设置背景颜色
//    QString background = "background-color:";
//    switch (color) {
//    case 0:
//        // 灰色
//        background += "rgb(190,190,190)";
//        break;
//    case 1:
//        // 红色
//        background += "rgb(255,90,90)";
//        break;
//    case 2:
//        // 绿色
//        background += "rgb(0,255,100)";
//        break;
//    case 3:
//        // 黄色
//        background += "rgb(255,255,0)";
//        break;
//    default:
//        break;
//    }

//    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
//    label->setStyleSheet(SheetStyle);
}

void Program::Drawing()
{
    QString labeltext("font: 40 12pt '微软雅黑 Light';"
                      "font-weight: bold;"
                      "border:none;"
                      "color:rgb(255,255,255);"
                      "background-color: transparent;");
    ui->labeldelt->setText("未开启");
    //ui->labelswieps->setText("未开启");
    //ui->labelsen->setText("未检测到");
    ui->labellas->setText("未开启");
    //ui->labeldetcode->setText("未开启");
    ui->labeldetmark->setText("未开启");
    ui->labeleli->setText("未开启");
    //ui->labelins->setText("未开启");
    ui->labellift->setText("未开启");

    ui->centralwidget->setStyleSheet("background-color: rgb(0,0,185);");

    ui->Btnshow->setFixedSize(100, 35);
    ui->Btnshow->setStyleSheet("QPushButton{font: 22 14pt '微软雅黑 Light';"
                                "color: rgb(255,255,255);"
                                "background-color: rgba(100,159,237,90);"
                                "border: none;border-radius:15px;}"
                                "QPushButton:hover{background-color:rgba(80,139,217,70) ;}"//hover
                                "QPushButton:pressed{background-color:rgba(120,179,255,110);}");//pressed

    ui->Btnstop->setFixedSize(100, 35);
    ui->Btnstop->setStyleSheet("QPushButton{font: 22 14pt '微软雅黑 Light';"
                               "color: rgb(255,255,255);"
                               "background-color: rgba(100,159,237,90);"
                               "border: none;border-radius:15px;}"
                               "QPushButton:hover{background-color:rgba(80,139,217,70) ;}"//hover
                               "QPushButton:pressed{background-color:rgba(120,179,255,110);}");//pressed



    ui->labeltopic->setStyleSheet("font: 50 25pt '微软雅黑 Light';"
                                      "font-weight: bold;"
                                      "border:none;"
                                      "color:rgb(255,255,255);"
                                      "background-color: transparent;");

    ui->label_2->setStyleSheet(labeltext);

    //ui->label_3->setStyleSheet(labeltext);

    //ui->label_4->setStyleSheet(labeltext);

    ui->label_5->setStyleSheet(labeltext);

    ui->label_6->setStyleSheet(labeltext);

    ui->label_7->setStyleSheet(labeltext);

    //ui->label_8->setStyleSheet(labeltext);

    ui->label_9->setStyleSheet(labeltext);

    //ui->label_10->setStyleSheet(labeltext);

    ui->label_11->setStyleSheet(labeltext);

    //ui->label_12->setStyleSheet(labeltext);

    ui->labeldelt->setStyleSheet(labeltext);
    ui->labeldetmark->setStyleSheet(labeltext);
    ui->labeleli->setStyleSheet(labeltext);
    ui->labelepson->setStyleSheet(labeltext);
    ui->labellas->setStyleSheet(labeltext);
    ui->labellift->setStyleSheet(labeltext);

    ui->labelstate->setStyleSheet("font: 50 22pt '微软雅黑 Light';"
                                  "font-weight: bold;"
                                  "border:none;"
                                  "color:rgb(255,255,255);"
                                  "background-color: transparent;");
    ui->Btncon->setFixedSize(110, 32);
    ui->Btncon->setStyleSheet("QPushButton{font: 25 16pt '微软雅黑 Light';"
                                "color: rgb(255,255,255);"
                                "background-color: rgb(0,190,250);"
                                "border: none;border-radius:15px;}"
                                "QPushButton:hover{background-color:rgb(0,165,225) ;}"//hover
                                "QPushButton:pressed{background-color:rgb(25,215,255);}");//pressed

    ui->Btnenter->setFixedSize(110, 32);
    ui->Btnenter->setStyleSheet("QPushButton{font: 25 16pt '微软雅黑 Light';"
                                "color: rgb(255,255,255);"
                                "background-color: rgb(0,190,250);"
                                "border: none;border-radius:15px;}"
                                "QPushButton:hover{background-color:rgb(0,165,225) ;}"//hover
                                "QPushButton:pressed{background-color:rgb(25,215,255);}");//pressed




}
