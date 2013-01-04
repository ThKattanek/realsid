//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: main.cpp                              //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 04.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QBitmap>
#include "mainwindow.h"

#undef main

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/img/splash");
    QSplashScreen *splash = new QSplashScreen();
    splash->setPixmap(pixmap);;
    splash->setMask(pixmap.mask());
    splash->show();

    MainWindow w;

    QTimer::singleShot(2500, splash, SLOT(close()));
    QTimer::singleShot(2500, &w, SLOT(show()));

    return a.exec();
}
