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
    ftd2xx {
        INCLUDEPATH += "deps/linux/release"
        LIBS += -L"deps/linux/release/build/x86_64" -lftd2xx
    } else {
        LIBS += -lftdi
    }
}

win32 {
    INCLUDEPATH += "deps/win"
    LIBS += -L"deps/win/CDM v2.08.30 WHQL Certified/i386" -lftd2xx
}

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/ftdipinout.cpp \
    src/dumpdialog.cpp \
    src/hexvalidator.cpp \
    src/ft232r_spi.cpp

HEADERS  += src/mainwindow.h \
           src/ftdipinout.h \
    ../../nrf24l01_lib/nrf24l01.h \
    src/dumpdialog.h \
    src/hexvalidator.h \
    src/ft232r_spi.h

FORMS    += ui/mainwindow.ui \
            ui/FTDI_Pinout.ui \
    ui/dumpdialog.ui

