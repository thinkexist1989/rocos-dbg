#include "rocosmainwindow.h"

#include <QApplication>
#include <QRibbon/QRibbon.h>

#include <SplashScreen.h>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RocosMainWindow w;

    a.setWindowIcon(QIcon(":/icon.ico"));

    QPixmap pixmap(":/res/rocos-dbg-logging.jpg");
    QPixmap scaledPixmap = pixmap.scaled(700,700, Qt::KeepAspectRatio);
    SplashScreen* splash = new SplashScreen(scaledPixmap, 10000);
    splash->show();

    for(int i = 0; i <= 100; i++) {
        splash->setProgress(i);
        QThread::usleep(10000);
    }

    QRibbon::install(&w);
    w.show();
    splash->finish(&w);
    return a.exec();
}
