#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QString>
#include <QTime>
#include "connecttogen.h"
#include "connecttoplc.h"
#include "program.h"
#include "operation.h"
#include "camera.h"
#include "set.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Drawing();
    void Update();

signals:
    void Start(int command);
    void Stop(int command);
    void Emergy1(int command);
    void Emergy2(int command);
    void Reset(int command);
private slots:

    void TimeReset();
    //    void InitializePosition();//初始化界面
    //    void KeyReleaseEvent(QKeyEvent *event);
    //    void KeyPressEvent(QKeyEvent *event);
    //    void PaintEvent(QPaintEvent *);//均为控制信号函数
private:
    Ui::MainWindow *ui;
    ConnectToGen *connecttogen;
    ConnectToPlc *connecttoplc;
    Program *program;
    Operation *operation;
    Set *set;
    QTimer *TimerReset;

};

#endif // MAINWINDOW_H
