#include "mainwindow.h"
#include "connecttogen.h"
#include "connecttoplc.h"
#include "program.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QApplication::setWindowIcon(QIcon(":icons/3.ico"));
    a.setWindowIcon(QIcon(":icons/3.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}
