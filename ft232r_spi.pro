#-------------------------------------------------
#
# Project created by QtCreator 2014-01-06T16:05:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ft232r_spi
TEMPLATE = app

unix {
    INCLUDEPATH += "../linux/release"
    LIBS += -L../linux/release/build/x86_64 -lftd2xx
}

win32 {
    INCLUDEPATH += "../win"
    LIBS += -L"../win/CDM v2.08.30 WHQL Certified/i386" -lftd2xx
}

SOURCES += main.cpp\
        mainwindow.cpp \
        ftdipinout.cpp \
    dumpdialog.cpp

HEADERS  += mainwindow.h \
           ftdipinout.h \
    ../../nrf24l01_lib/nrf24l01.h \
    dumpdialog.h

FORMS    += mainwindow.ui \
            FTDI_Pinout.ui \
    dumpdialog.ui

