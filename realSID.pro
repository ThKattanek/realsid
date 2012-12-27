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
    oscclass.cpp

HEADERS  += mainwindow.h \
    sidclass.h \
    oscclass.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    liesmich.txt \
    SID-Artikel.odt

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





