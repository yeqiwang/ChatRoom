#include "datahandle.h"
#include "network/network.h"

#include <QJsonObject>

DataHandle::DataHandle(QObject *parent)
    : QObject{parent}
{
    connect(Network::getObject(), SIGNAL(readJsonFromClient(QJsonObject,int)),
            this, SLOT(readJsonFromClientSlot(QJsonObject,int)));
}

DataHandle *DataHandle::getObject()
{
    static DataHandle dh;
    return &dh;
}

void DataHandle::readJsonFromClientSlot(const QJsonObject &obj, int id)
{
    QString type = obj.value("type").toString();
    if (type == "meg"){
        QJsonObject reObj;
        reObj.insert("type", "reMeg");

        QString username = obj.value("username").toString();
        QStringList usernames = Network::getObject()->getUsernameList();
        int re = usernames.indexOf(username);
        re = re == -1? 0: 1;
        reObj.insert("re", re);
        Network::getObject()->sendJsonToClient(reObj, id);

        if (re) {
            QJsonObject reObj1;
            reObj1.insert("type", "newMeg");
            reObj1.insert("username", Network::getObject()->getUsername(id));
            reObj1.insert("meg", obj.value("meg").toString());
            Network::getObject()->sendJsonToClient(reObj1, username);
        }
    }
    else if (type == "bind"){
        QJsonObject reObj;
        reObj.insert("type", "reBind");

        QString username = obj.value("username").toString();
        QStringList usernames = Network::getObject()->getUsernameList();
        int re = usernames.indexOf(username);
        re = re == -1? 1: 0;
        if (re)
            Network::getObject()->setUsername(id, username);

        reObj.insert("re", re);
        Network::getObject()->sendJsonToClient(reObj, id);
    }
}
