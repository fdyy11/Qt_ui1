#ifndef OPERATION_H
#define OPERATION_H

#include <QMainWindow>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QLabel>

namespace Ui {
class Operation;
}

class Operation : public QMainWindow
{
    Q_OBJECT

public:
    explicit Operation(QWidget *parent = nullptr);
    ~Operation();
    void InitButton();
    void InitLed();
    void Drawing();
    void setLED(QLabel* label, int color, int size);
    void Btncom();
signals:
    void Btnclicked(int command);
private slots:
    void onRadioButtonClicked1();
    void onRadioButtonClicked2();
    void onRadioButtonClicked3();
    void onRadioButtonClicked4();

    void onRadioButtonClicked5();
    void onRadioButtonClicked6();
    void onRadioButtonClicked7();
    void onRadioButtonClicked8();

    void onRadioButtonClicked9();
    void onRadioButtonClicked10();
    void onRadioButtonClicked11();
    void onRadioButtonClicked12();

    void onRadioButtonClicked13();
    void onRadioButtonClicked14();

private:
    Ui::Operation *ui;
    QButtonGroup *btnGroup1,*btnGroup2,*btnGroup3,*btnGroup4,*btnGroup5,*btnGroup6,
        *btnGroup7,*btnGroup8,*btnGroup9,*btnGroup10,*btnGroup11,*btnGroup12,*btnGroup13,*btnGroup14;
};

#endif // OPERATION_H
