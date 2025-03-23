#ifndef SET_H
#define SET_H

#include <QMainWindow>
#include "connecttogen.h"
#include "connecttoplc.h"
#include "camera.h"

namespace Ui {
class Set;
}

class Set : public QMainWindow
{
    Q_OBJECT

public:
    explicit Set(QWidget *parent = nullptr);
    ~Set();
    void Drawing();
signals:
    void MidControl(int command);
    void CameraRead();
    void Programshow();
    void Programstop();
public slots:
    void CameraSet();
    void ShowPLC();
    void StopPLC();
private slots:
    void OperationControl(int command);


private:
    Ui::Set *ui;
    ConnectToGen *connecttogen;
    ConnectToPlc *connecttoplc;
};

#endif // SET_H
