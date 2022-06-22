#include "interface.h"
#include "network/network.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Network::getObject()->connectToServer("xxx.xxx.xxx.xxx", xx);
    Interface::getObject()->show();

    return a.exec();
}
