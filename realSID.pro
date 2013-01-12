#-------------------------------------------------
#
# Project created by Thorsten Kattanek
#
#-------------------------------------------------

QT       += core gui

TARGET = realSID
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    sidclass.cpp \
    oscclass.cpp \
    siddump.cpp \
    envclass.cpp \
    filterclass.cpp \
    sequenzerclass.cpp \
    sequenzerwindow.cpp \
    spinbox.cpp \
    notenedit.cpp

HEADERS  += mainwindow.h \
    sidclass.h \
    oscclass.h \
    siddump.h \
    waves.h \
    envclass.h \
    filterclass.h \
    filtercalc.h \
    sequenzerclass.h \
    sequenzerwindow.h \
    spinbox.h \
    notenedit.h

FORMS    += mainwindow.ui \
    sequenzerwindow.ui

OTHER_FILES += \
    liesmich.txt \
    SID-Artikel.odt \
    referenz/dane-crest-mos-8580r5.ogg \
    referenz/dane-crest.sdp \
    sid-wave-capture.asm \
    SID-Artikel.pdf \
    img/mos8580-pic.png \
    img/splash.png \
    SequenzerClass.odt \
    SequenzerClass.pdf

win32 {
INCLUDEPATH = C:\QtSDK\mingw\include\SDL
TARGET = start_win32
DESTDIR = "bin"
LIBS += -lmingw32 -lSDLmain -lSDL -lSDL_gfx
}

linux-g++{
TARGET = start_linux32
DESTDIR = "bin"
LIBS += -lSDL -lSDL_gfx
}

linux-g++-64{
INCLUDEPATH = /usr/include/SDL
TARGET = start_linux64
DESTDIR = "bin"
LIBS += -lSDL -lSDL_gfx
}

RESOURCES += \
    res.qrc



































