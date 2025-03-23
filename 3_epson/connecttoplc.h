#ifndef CONNECTTOPLC_H
#define CONNECTTOPLC_H

#include <QMainWindow>
#include<QHostInfo>
#include<QTcpServer>
#include<QTcpSocket>
#include <QTimer>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include "modbus.h"


namespace Ui {
class ConnectToPlc;
}

class ConnectToPlc : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectToPlc(QWidget *parent = nullptr);
    ~ConnectToPlc();
    void Drawing();//界面绘制
    void initWidget();
    void Datawrite(int startAdd,int writeNum);
    void Dataread(int startAdd,int readNum);
signals:
    void SendToGen();
public slots:
    void slot_stateChanged(bool flag);
    //void slot_readCoils(QVector<quint16> vAllData);
    void slot_readRegisters(int resultNum);
    void slot_readRegisters5(int resultNum1,int resultNum2,int resultNum3,int resultNum4,int resultNum5,int resultNum6);

    void on_Btncon_clicked();
    void on_BtnreadR_clicked();
    void on_BtnwriteR_clicked();
//    void on_BtnreadC_clicked();
//    void on_BtnwriteC_clicked();
//    void on_Btnuse_clicked();

    void Receive();
    void Show();
    void Stop();
    void Control(int command);

    void Gen_Control();
private:
    Ui::ConnectToPlc *ui;
    Modbus *m_myModsbus;
    QTimer *frameTimer1;
};

#endif // CONNECTTOPLC_H
