#include "network.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

Network::Network(QObject *parent)
    : QObject{parent}
{
    server = new QTcpServer;
    connect(server,SIGNAL(newConnection()), this,SLOT(newConnectionSlot()));
}

int Network::getUserIndex(int id)
{
    for (quint16 i = 0; i < clientList.length(); i++) {
        if (clientList.at(i)->id == id){
            return i;
        }
    }
    return -1;
}

int Network::getUserIndex(QString username)
{
    for (quint16 i = 0; i < clientList.length(); i++) {
        if (clientList.at(i)->username == username){
            return i;
        }
    }
    return -1;
}

int Network::getUserIndex(QTcpSocket *s)
{
    for (quint16 i = 0; i < clientList.length(); i++) {
        if (clientList.at(i)->socket == s){
            return i;
        }
    }
    return -1;
}

QString Network::getUsername(int id)
{
    for (quint16 i = 0; i < clientList.length(); i++) {
        if (clientList.at(i)->id == id){
            return clientList.at(i)->username;
        }
    }
    return "";
}

void Network::setUsername(int id, QString username)
{
    for (quint16 i = 0; i < clientList.length(); i++) {
        if (clientList.at(i)->id == id){
            clientList.at(i)->username = username;
        }
    }
}

QStringList Network::getUsernameList()
{
    QStringList s;
    for (int i = 0; i < clientList.length(); i++){
        s.append(clientList.at(i)->username);
    }
    return s;
}

int Network::getUserId()
{
    static int i = 0;
    return i++;
}

bool Network::startServer(quint16 port)
{
    if(server->isListening()){
        server->close();
        for (int i = 0; i < clientList.length(); i++) {
            disconnect(clientList.at(i)->socket, nullptr, nullptr, nullptr);
            clientList.at(i)->socket->abort();
            delete clientList.at(i);
        }
        clientList.clear();
    }
    return server->listen(QHostAddress::AnyIPv4, port);
}

bool Network::sendJsonToClient(const QJsonObject &obj, int id)
{
    QByteArray data;
    data = QJsonDocument(obj).toJson();
    int index = getUserIndex(id);

    if (index == -1){
        return false;
    }
    return clientList.at(index)->socket->write(data) == -1? false: true;
}

bool Network::sendJsonToClient(const QJsonObject &obj, QString username)
{
    QByteArray data;
    data = QJsonDocument(obj).toJson();
    int index = getUserIndex(username);

    if (index == -1){
        return false;
    }
    return clientList.at(index)->socket->write(data) == -1? false: true;
}

void Network::readyReadSlot()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());

    // 查看该用户是否在用户列表
    int index = getUserIndex(socket);
    if(index == -1)
       return;

    QByteArray data = socket->readAll();
    UserClientInfo *info = clientList.at(index);
    for(int i = 0; i < data.length(); i++)
    {
       if(info->readBuffer.length() > 0 && data.at(i) == '{'
               && info->lastChar != ':' && info->lastChar != '['
               && info->lastChar != ',')
       {
           //接收到开头，之前的数据不完整丢弃
           info->readBuffer.clear();
           info->readBuffer.append(data.at(i));
           info->lastChar = data.at(i);
           info->braNum = 1;
           continue;
       }
       if(data.at(i) == '{')
       {
           info->braNum++;
           info->readBuffer.append(data.at(i));
           info->lastChar = '{';
           continue;
       }
       else if(data.at(i) == '}')
       {
           info->braNum--;
           info->readBuffer.append(data.at(i));
           info->lastChar = '}';
           if(info->braNum == 0)
           {
               //一条完整数据
               QJsonObject obj = QJsonDocument::fromJson(info->readBuffer)
                       .object();
               emit readJsonFromClient(obj, info->id);
               //数据清空
               info->readBuffer.clear();
               info->braNum = 0;
           }
           continue;
       }
       else
       {
           info->readBuffer.append(data.at(i));
           if(data.at(i) != ' ' && data.at(i) != '\n'
              && data.at(i) != '\t' && data.at(i) != 0x0a)
               info->lastChar = data.at(i);
           continue;
       }
    }
}

void Network::newConnectionSlot()
{
    qDebug() << "new client comes.";
    QTcpSocket *socket = server->nextPendingConnection();

    // 为新连接绑定槽函数
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));

    // 创建索引
    UserClientInfo *info = new UserClientInfo;
    info->id = getUserId();
    info->braNum = 0;
    info->lastChar = ' ';
    info->readBuffer.clear();
    info->socket = socket;

    clientList.append(info);
}

void Network::disconnectedSlot()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    for (int i = 0; i < clientList.length(); i++) {
        if (clientList.at(i)->socket == socket){
            delete clientList.at(i);
            clientList.removeAt(i);
            return;
        }
    }
}

Network *Network::getObject()
{
    static Network n;
    return &n;
}
