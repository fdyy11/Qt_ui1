#ifndef PROGRAM_H
#define PROGRAM_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include "camera.h"

namespace Ui {
class Program;
}

class Program : public QMainWindow
{
    Q_OBJECT

public:
    explicit Program(QWidget *parent = nullptr);
    ~Program();
    void Drawing();
    void setLED(QLabel* label, int color, int size);
    void openDatabases();
signals:
    void ConClicked();

    void CameraRead();
    void StartShow();
    void StopShow();


private slots:
    void Update();
    void EnterDatabase();
    void CameraEncode();

    void on_Btncon_clicked();
    void on_Btnshow_clicked();
    void on_Btnstop_clicked();
private:
    Ui::Program *ui;
    QTimer *frameTimer1;
    Camera *camera;
};

#endif // PROGRAM_H
