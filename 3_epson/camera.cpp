#include "camera.h"
#include "ui_camera.h"
#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QtUiTools/QUiLoader>
#include <QtWidgets>
#include <opencv2/opencv.hpp>

#define port0 4100 //接受视频流数据
#define port1 4200 //接受文本数据



cv::Mat Image0;

cv::Mat lastReceivedImage; // 用于保存上一帧接收到的图像
bool whiteScreenDetected = false; // 是否检测到白屏

std::string flagSubstring;

int k=0;

extern int flag;
extern int numberr;

int E_box=0,E_detect=0;


Camera::Camera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Camera)
{
    ui->setupUi(this);
    setFixedSize(480, 520); // 设置窗口大小
    move(20,0);
    ui->videoLabel->setFixedSize(480,320);
    Drawing();
    tcpserver=new QTcpServer(this);
    tcpserver2=new QTcpServer(this);

    tcpserver->listen(QHostAddress::Any, port0);
    tcpserver2->listen(QHostAddress::Any, port1);

    tcpsocket=nullptr;
    tcpsocket2=nullptr;

    connect( tcpserver,&QTcpServer::newConnection,this, &Camera::ConnectToClient);
    connect( tcpserver2,&QTcpServer::newConnection,this, &Camera::ConnectToClient2);

    frameTimer1 = new QTimer(this);
    connect(frameTimer1, &QTimer::timeout, this, &Camera::ReadInformation);
    frameTimer1->start(105); // 20帧/秒（1000毫秒/20帧）

    frameTimer2 = new QTimer(this);
    connect(frameTimer2, &QTimer::timeout, this, &Camera::ReadInformation2);
    frameTimer2->start(100); // 20帧/秒（1000毫秒/20帧）

    frameTimer3 = new QTimer(this);
    connect(frameTimer3, &QTimer::timeout, this, &Camera::processNextFrame);
    frameTimer3->start(100); // 20帧/秒（1000毫秒/20帧）



}

Camera::~Camera()
{
    delete ui;
}


void Camera::ConnectToClient()
{
    //取出建立好的套接字
    tcpsocket = tcpserver->nextPendingConnection();


    QMessageBox::information(this,"notice","Connect to port0");

    ui->labelcon1->setText(QString("%1").arg(port0));
}

void Camera::ConnectToClient2()
{
    //取出建立好的套接字
    tcpsocket2 = tcpserver2->nextPendingConnection();
    //connect(tcpsocket2, &QTcpSocket::readyRead, this, &MainWindow::ReadInformation2);

    QMessageBox::information(this,"notice","Connect to port1");

    ui->labelcon2->setText(QString("%1").arg(port1));
}


//用于处理视频帧数据
void Camera::ReadInformation()
{

    if (tcpsocket && tcpsocket->bytesAvailable() > 0) {
        QByteArray temp = tcpsocket->readAll();
        std::vector<uchar> data(temp.begin(), temp.end());

        decodeImage(data,Image0);

    }
}

//用于处理文本数据
void Camera::ReadInformation2()
{

    if (tcpsocket2 && tcpsocket2->bytesAvailable() > 0) {
        QByteArray temp = tcpsocket2->readAll();
        std::vector<uchar> data(temp.begin(), temp.end());
        decodeMessage(data);

    }
}


bool Camera::isWhiteScreen(const cv::Mat &image)
{
    // 检查输入图像是否为空
    if (image.empty()) {
        // 图像为空，返回false或者采取适当的措施
        //qDebug() << "图像为空."<<i++;
        return true;
    }

    else return false;
}



void Camera::updateUIWithImage(const QImage& image)
{
    // 在 QLabel 中显示 QImage
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->videoLabel->setPixmap(pixmap);
    ui->videoLabel->setScaledContents(true); // 自动缩放 QLabel 内容以适应窗口大小
    ui->videoLabel->move(0,0);//设置显示位置
    ui->videoLabel->setFixedSize(640, 360);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置文本画笔和字体
    QPen pen(Qt::red, 2);
    painter.setPen(pen);
    QFont font;
    font.setPointSize(40);
    painter.setFont(font);


    painter.drawText(QRect(0, 0, pixmap.width(), pixmap.height()), Qt::AlignTop | Qt::AlignRight, QString("盒子数量: %1").arg(E_box));

    ui->videoLabel->setPixmap(pixmap);
}



void Camera::processNextFrame()
{
    //t++;
        // 检查图像是否为白屏
        if (!isWhiteScreen(Image0)) {
            // 如果不是白屏，保存上一帧接收到的图像
            lastReceivedImage = Image0;
            whiteScreenDetected = false;
        } else {
            whiteScreenDetected = true;
        }

        // 将OpenCV图像转换为QImage
        QImage qImage(Image0.data, Image0.cols, Image0.rows, Image0.step, QImage::Format_BGR888);

        // 显示图像，除非检测到白屏
        if (!whiteScreenDetected) {
            updateUIWithImage(qImage);
        }
        else if (!lastReceivedImage.empty()) {
        // 如果没有新的图像数据，但是有上一次接收到的图像，显示上次接收到的图像
        qDebug() << "没有新的图像数据"<<k++;
        QImage qImage(lastReceivedImage.data, lastReceivedImage.cols, lastReceivedImage.rows, lastReceivedImage.step, QImage::Format_BGR888);
        updateUIWithImage(qImage);
        }


}


// 从编码后的数据中解码图像
void Camera::decodeImage(const std::vector<uchar>& encodedData,cv::Mat& img) {
        std::string frameHeader = "AAAA";
        std::string frameFooter = "FFFF";

        std::string flag1 = "A01";//实时图片

        std::vector<uchar> imageBuffer;

        // 找到帧头及其相关标志位
        auto headerPos = std::search(encodedData.begin(), encodedData.begin() + 4, frameHeader.begin(), frameHeader.end());
        if (headerPos != encodedData.begin() + 4) {
        auto footerPos = std::search(encodedData.end() - 4, encodedData.end(), frameFooter.begin(), frameFooter.end());
        if (footerPos != encodedData.end())  // 找到帧尾
        {
            auto flagPos = headerPos + frameHeader.size();
            flagSubstring = std::string(flagPos, flagPos + 3);//提取标志位

            if (flagSubstring == flag1)
            {
                // 提取编码后的图像数据
                imageBuffer.insert(imageBuffer.end(), flagPos + flag1.size(), footerPos);

                // 解码并显示图像
                img = cv::imdecode(imageBuffer, cv::IMREAD_COLOR);
                qDebug() << "图片" << Qt::endl;
            }
            else
                qDebug() << "标志位1不匹配" << Qt::endl;

        }
        else
            qDebug() << "尾不匹配!" << Qt::endl;
        }
        else
        {
        qDebug() << "头不匹配" << Qt::endl;
        }

}


//解码文本数据
void Camera::decodeMessage(const std::vector<uchar>& encodedData) {

        std::string frameHeader = "AAAA";
        std::string frameFooter = "FFFF";
        std::string flag2 = "A02";
        std::string flag3 = "A03";

        std::string strEncodedData(encodedData.begin(), encodedData.end());

        size_t headerPos = strEncodedData.find(frameHeader);
        if (headerPos != std::string::npos) {
        size_t flagPos = headerPos + frameHeader.size();
        std::string decodedMessage;
        //


        if (strEncodedData.compare(flagPos, flag2.size(), flag2) == 0) {
            size_t footerPos = strEncodedData.find(frameFooter, flagPos);
            if (footerPos != std::string::npos) {
                decodedMessage = strEncodedData.substr(flagPos + flag2.size(), footerPos - (flagPos + flag2.size()));
                qDebug()<< "盒子个数:" << decodedMessage << Qt::endl;
                //赋值
                E_box= std::stof(decodedMessage)/2;
                ui->labelbox->setText(QString::number(E_box));
                flag=26;
                numberr=E_box;
                emit CameraData();
            }
            else {
                qDebug() << "未找到帧尾!"<< Qt::endl ;
            }
        }



        //
        else if (strEncodedData.compare(flagPos, flag3.size(), flag3) == 0) {
            size_t footerPos = strEncodedData.find(frameFooter, flagPos);
            if (footerPos != std::string::npos) {
                decodedMessage = strEncodedData.substr(flagPos + flag3.size(), footerPos - (flagPos + flag3.size()));
                qDebug()<< "检测瑕疵有无:" << decodedMessage << Qt::endl;
                //赋值
                E_detect=std::stof(decodedMessage);
                ui->labeldetect->setText(QString::number(E_detect));
                flag=27;
                numberr=E_detect;
                emit CameraData();
            }
            else {
                qDebug() << "未找到帧尾!"<< Qt::endl ;
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


void Camera::Drawing(){
        ui->centralwidget->setStyleSheet("background-color: rgba(190,190,190,100);");

        ui->videoLabel->setText("相机未接入");
        ui->videoLabel->setAlignment(Qt::AlignCenter);
        ui->videoLabel->setStyleSheet(  "font: 40 20pt '微软雅黑 Light';"
                                        "color: rgb(255,255,255);"
                                        "font-weight: bold;");

        ui->labelcon1->setText("无连接");
        ui->labelcon2->setText("无连接");
        ui->labelbox->setText("暂无");
        ui->labeldetect->setText("暂无");

        QString Flabel("font: 30 15pt '微软雅黑 Light';"
                       "font-weight: bold;"
                       "border:7pt;"
                       "color: rgb(255,255,255);"
                       "background-color: transparent;");

        ui->labelcon1->setStyleSheet(Flabel);
        ui->labelcon2->setStyleSheet(Flabel);
        ui->labelbox->setStyleSheet(Flabel);
        ui->labeldetect->setStyleSheet(Flabel);
        ui->label1->setStyleSheet(Flabel);
        ui->label2->setStyleSheet(Flabel);
        ui->label3->setStyleSheet(Flabel);







}
