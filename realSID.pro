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
    SequenzerClass.pdf \
    img/huellkurve.png

linux-g++-32{
DESTDIR = "bin/linux_32bit"
TARGET = realsid
LIBS += -lSDL -lSDL_gfx
}

linux-g++-64{
TARGET = realsid
DESTDIR = "bin/linux_64bit"
LIBS += -lSDL -lSDL_gfx
}

win32-g++-cross{
TARGET = realsid.exe
DESTDIR = "bin/win_x32"
LIBS += -lmingw32 -lSDLmain -lSDL -lSDL_gfx
}

win64-g++-cross{
TARGET = realsid.exe
DESTDIR = "bin/win_x64"
LIBS += -lmingw32 -lSDLmain -lSDL -lSDL_gfx
}    

RESOURCES += \
    res.qrc




































