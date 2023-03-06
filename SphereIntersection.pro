QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    paraminputdialog.cpp \
    point2d.cpp \
    point3d.cpp \
    shader.cpp \
    sphere.cpp

HEADERS += \
    glwidget.h \
    mainwindow.h \
    paraminputdialog.h \
    point2d.h \
    point3d.h \
    shader.h \
    sphere.h

FORMS += \
    mainwindow.ui \
    paraminputdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shader/shader.frag \
    shader/shader.vert
