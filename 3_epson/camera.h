#ifndef CAMERA_H
#define CAMERA_H

#include <QMainWindow>

#include <QTcpServer>
#include <QTcpSocket>

#include <QImage>
#include <QLabel>
#include <QTimer>
#include <opencv2/opencv.hpp>


namespace Ui {
class Camera;
}

class Camera : public QMainWindow
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);
    ~Camera();
    void setLED(QLabel* label, int color, int size);
    void decodeImage(const std::vector<uchar>& encodedData,cv::Mat& img);
    void decodeMessage(const std::vector<uchar>& encodedData);
    void Drawing();
signals:
    void CameraData();
public slots:
    void updateUIWithImage(const QImage& image);//处理空白数据帧
    void processNextFrame();//实时更新视频帧
    bool isWhiteScreen(const cv::Mat& image);//判断当前视频帧有无数据

    void ConnectToClient();
    void ConnectToClient2();

    void ReadInformation();
    void ReadInformation2();
private:
    Ui::Camera *ui;

    QImage videoFrame;
    QTimer *frameTimer1;
    QTimer *frameTimer2;
    QTimer *frameTimer3;

    QTcpServer *tcpserver;
    QTcpServer *tcpserver2;

    QTcpSocket *tcpsocket;
    QTcpSocket *tcpsocket2;

};

#endif // CAMERA_H
