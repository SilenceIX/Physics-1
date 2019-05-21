#include "mainwindow.h"
#include <signal.h>
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pix( ":/splash.jpg" );
    QSplashScreen splashScreen(pix);
    splashScreen.show();
    a.processEvents();
    QThread::sleep(2);
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    splashScreen.finish(&w);
    return a.exec();
}
