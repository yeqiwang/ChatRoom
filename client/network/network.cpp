#include "network.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

Network::Network(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket;
    lastChar = 0;
    braNum = 0;
    readBuffer = "";
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyReadSlot()));
    connect(this, SIGNAL(readJsonFromServer(QJsonObject)), this, SLOT(readJsonFromServerSlot(QJsonObject)));
}

Network *Network::getObject()
{
    static Network n;
    return &n;
}

void Network::connectToServer(QString ipAddr, quint16 port)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->abort();
    }
    socket->connectToHost(ipAddr, port);
}

bool Network::sendJsnToServer(const QJsonObject &obj)
{
    if (socket->state() == QTcpSocket::ConnectedState){
        QByteArray data = QJsonDocument(obj).toJson();
        if (socket->write(data))
            return true;
    }
    return false;
}

void Network::socketReadyReadSlot()
{
    QByteArray data = socket->readAll();

    Network *info = this;
    for(int i = 0 ; i<data.length();i++){
       if(info->readBuffer.length() > 0 && data.at(i)=='{'
               && info->lastChar != ':'&& info->lastChar != '['
               && info->lastChar != ','){
           info->readBuffer.clear();
           info->readBuffer.append(data.at(i));
           info->braNum = 1;
           continue;
       }
       if(data.at(i) == '{'){
           info->braNum++;
           info->readBuffer.append(data.at(i));
           info->lastChar = data.at(i);
           continue;
       }
       else if(data.at(i) == '}'){
           info->braNum--;
           info->readBuffer.append(data.at(i));
           info->lastChar = data.at(i);
           if(info->braNum ==0 ){
               QJsonObject obj = QJsonDocument::fromJson(readBuffer).object();
               emit readJsonFromServer(obj);
               info->readBuffer.clear();
               info->braNum = 0;
           }
           continue;
       }
       else{
           info->readBuffer.append(data.at(i));
           if(data.at(i) != ' ' && data.at(i) != '\n')
               info->lastChar = data.at(i);
           continue;
       }
    }
}

void Network::readJsonFromServerSlot(const QJsonObject &obj)
{
    qDebug() << obj;
    QString type = obj.value("type").toString();
    if (type == "reMeg"){
        int re = obj.value("re").toInt();
        emit reMeg(re);
    }
    else if (type == "reBind"){
        int re = obj.value("re").toInt();
        emit reBind(re);
    }
    else if (type == "newMeg") {
        QString username = obj.value("username").toString();
        QString meg = obj.value("meg").toString();
        emit newMeg(username, meg);
    }
}

