#-------------------------------------------------
#
# Project created by QtCreator 2012-08-01T18:08:15
#
#-------------------------------------------------

QT       += core gui

TARGET = Rendering_test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cloudeoctrl.cpp \
    renderingwidget.cpp \
    cdohelpers.cpp

HEADERS  += mainwindow.h \
    cloudeoctrl.h \
    renderingwidget.h \
    cdohelpers.h

FORMS    += mainwindow.ui

CLOUDEO_SDK_HOME = $$PWD/../../cloudeo_sdk

#DEFINES += "CLOUDEO_SDK_HOME="\"$${CLOUDEO_SDK_HOME}\""

win32: LIBS += -L$${CLOUDEO_SDK_HOME} -lcdo_sdk

INCLUDEPATH += $${CLOUDEO_SDK_HOME}
DEPENDPATH += $${CLOUDEO_SDK_HOME}
