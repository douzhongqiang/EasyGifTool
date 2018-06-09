#-------------------------------------------------
#
# Project created by QtCreator 2018-05-21T22:00:43
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NormalQtDemo
TEMPLATE = app
RC_ICONS = app.ico

CONFIG += c++11
MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/bin

SOURCES += \
    core/main.cpp \
    UIBase/UIBaseWindow.cpp \
    Demos/SignalsAndSlot.cpp \
    UIBase/UIGlobalTool.cpp \
    Demos/ThreadSigAndSlots.cpp \
    Demos/BezierCurve.cpp \
    Demos/GifCreater.cpp \
    ToolBase/gif_h/gif.cpp \
    Demos/GifBuilderWidget.cpp \
    Demos/ScreenShortWidget.cpp \
    Demos/ImageView.cpp \
    UIBase/UIGlobalConfig.cpp \
    UIBase/UIMessageBox.cpp \
    UIBase/UIProgressWindow.cpp

HEADERS  += widget.h \
    UIBase/UIBaseWindow.h \
    Demos/SignalsAndSlot.h \
    UIBase/UIGlobalTool.h \
    Demos/ThreadSigAndSlots.h \
    Demos/BezierCurve.h \
    ToolBase/gif_h/gif.h \
    Demos/GifCreater.h \
    Demos/GifBuilderWidget.h \
    Demos/ScreenShortWidget.h \
    Demos/ImageView.h \
    UIBase/UIGlobalConfig.h \
    UIBase/UIMessageBox.h \
    UIBase/UIProgressWindow.h
