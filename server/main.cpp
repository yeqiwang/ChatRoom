#include <QCoreApplication>
#include "network/network.h"
#include "dataHandle/datahandle.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (Network::getObject()->startServer(xx)){
        qDebug() << "Server is working, listening the port of xx.";
    }
    else {
        qDebug() << "Server strat failed";
        return a.exec();
    }

    DataHandle::getObject();

    return a.exec();
}
