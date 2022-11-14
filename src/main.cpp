#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTimer>

#include "Threads.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTimer timer;
    timer.setSingleShot(true);

    QEventLoop loop;
    ThreadContainer threads(10);

    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    QObject::connect(&threads, SIGNAL(ThreadsCompleted()), &loop, SLOT(exit()) );
    timer.start(5000);
    threads.StartThreads();
    loop.exec();

    qDebug() << "Ultimate answer: " << threads.GetString();
    return 0;
}
