QT       += core gui network sql serialbus serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    camera.cpp \
    connecttogen.cpp \
    connecttoplc.cpp \
    main.cpp \
    mainwindow.cpp \
    modbus.cpp \
    operation.cpp \
    program.cpp \
    set.cpp

HEADERS += \
    camera.h \
    connecttogen.h \
    connecttoplc.h \
    mainwindow.h \
    modbus.h \
    operation.h \
    program.h \
    set.h

FORMS += \
    camera.ui \
    connecttogen.ui \
    connecttoplc.ui \
    mainwindow.ui \
    operation.ui \
    program.ui \
    set.ui


#QT += multimedia multimediawidgets
#QT += charts
#QT += opengl
#QT += 3dcore 3drender 3dextras
#QT += multimedia

INCLUDEPATH +=D:/Wjy/opencv/for_qt/install/include \
              D:/Wjy/opencv/for_qt/install/include/opencv \
              D:/Wjy/opencv/for_qt/install/include/opencv2

LIBS  +=-L D:\Wjy\opencv\for_qt\install\x64\mingw\lib\libopencv_*.dll.a


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icons/1.ico

RESOURCES += \
    icons/3.ico
    icons/1.ico
    icons/epson.png

RC_ICONS = 3.ico


