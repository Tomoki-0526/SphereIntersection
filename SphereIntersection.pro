QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    drawtask.cpp \
    drawtaskmanager.cpp \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    paraminputdialog.cpp \
    sphere.cpp

HEADERS += \
    drawtask.h \
    drawtaskmanager.h \
    glwidget.h \
    mainwindow.h \
    paraminputdialog.h \
    sphere.h

FORMS += \
    mainwindow.ui \
    paraminputdialog.ui

LIBS += \
    -lOpenGL32 \
    -lGlU32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shader/triangle.frag \
    shader/triangle.vert \
    shader/v1.frag \
    shader/v1.vert
