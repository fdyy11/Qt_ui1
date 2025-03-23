#ifndef CONNECTTOGEN_H
#define CONNECTTOGEN_H

#include <QMainWindow>
#include<QHostInfo>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include <QString>
#include <QTimer>

namespace Ui {
class ConnectToGen;
}

class ConnectToGen : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectToGen(QWidget *parent = nullptr);
    ~ConnectToGen();
    void Drawing();
    void decodeMessage(const std::vector<uchar>& encodedData);
    void encodeMessage();
signals:
    void GenControl();
public slots:
    void TCPClient1();//通讯建立
    void TCPClient2();//通讯建立

    void ReadInformation();//上位机信息读取

    void Connected();
    void SendData();
    void Send_Gen();
    //    void DataEcode(QString *data,const std::vector<uchar>& encodedData);//数据编码
    //    void DataTransfer(QString *data);//数据传输给上位机

    //    void Drawing();//界面绘制

private:
    Ui::ConnectToGen *ui;
    QTcpSocket *tcpClient1;
    QTcpSocket *tcpClient2;

//    QTcpSocket *tcpsocket;
//    QTcpSocket *tcpsocket2;

    QTimer *frameTimer1;

};

#endif // CONNECTTOGEN_H
