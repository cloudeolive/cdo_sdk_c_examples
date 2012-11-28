#-------------------------------------------------
#
# Project created by QtCreator 2012-08-05T10:42:26
#
#-------------------------------------------------

QT       += core gui

TARGET = adl_sample_app
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
    appcontroller.h \
    cryptlite/sha256.h \
    cryptlite/sha1.h \
    cryptlite/hmac.h \
    cryptlite/base64.h

FORMS    += cdosampleappwindow.ui

BOOST_HOME = $$PWD/../../../adl_libs/include/boost-1_49
ADL_HOME = $$PWD/../AddLive_sdk-win

win32: LIBS += -L$$ADL_HOME -lcdo_sdk

INCLUDEPATH += $$ADL_HOME
INCLUDEPATH += $$BOOST_HOME
DEPENDPATH += $$PWD/../../../smbuild/native_api/cloudeo_sdk_1.16.2.1
