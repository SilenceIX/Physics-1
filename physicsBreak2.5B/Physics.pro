#-------------------------------------------------
#
# Project created by QtCreator 2019-03-20T21:00:47
#
#-------------------------------------------------

QT       += core gui widgets 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Physics
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 resources_big


QMAKE_CXXFLAGS_RELEASE -= -O2

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    model4/bullet.cpp \
    model4/materialpoint.cpp \
    model4/measurer.cpp \
    model4/object.cpp \
    model4/pendulum.cpp \
    movecamerainroom.cpp \
    help.cpp \
    sup.cpp \
    model1.cpp \
    qcustomplot.cpp \
    plot.cpp \
    model2.cpp \
    model3.cpp \
    model4.cpp \
    model5.cpp \
    model6.cpp \
    model7.cpp \
    model4/plot4.cpp \
    model8.cpp

HEADERS += \
    mainwindow.h \
    help.h \
    model4/bullet.h \
    model4/materialpoint.h \
    model4/measurer.h \
    model4/object.h \
    model4/pendulum.h \
    model4/physics.h \
    models.h \
    qcustomplot.h \
    plot.h \
    model4/plot4.h

FORMS += \
        mainwindow.ui \
    help.ui \
    plot.ui \
    plot4.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
        resq.qrc
