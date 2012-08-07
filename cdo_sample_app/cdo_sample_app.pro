#-------------------------------------------------
#
# Project created by QtCreator 2012-08-05T10:42:26
#
#-------------------------------------------------

QT       += core gui

TARGET = cdo_sample_app
TEMPLATE = app


SOURCES += main.cpp\
        cdosampleappwindow.cpp \
    renderingwidget.cpp \
    cloudeoctrl.cpp \
    cdohelpers.cpp \
    appcontroller.cpp

HEADERS  += cdosampleappwindow.h \
    renderingwidget.h \
    cloudeoctrl.h \
    cdohelpers.h \
    appcontroller.h

FORMS    += cdosampleappwindow.ui

BOOST_HOME = $$PWD/../../../smlibs/include/boost-1_49
CDO_HOME = $$PWD/../cloudeo_sdk

win32: LIBS += -L$$PWD/../../../smbuild/native_api/cloudeo_sdk_1.16.2.1/ -lcdo_sdk

INCLUDEPATH += $$CDO_HOME
INCLUDEPATH += $$BOOST_HOME
DEPENDPATH += $$PWD/../../../smbuild/native_api/cloudeo_sdk_1.16.2.1
