#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

typedef struct
{
    QTcpSocket *socket;
    int id;
    int braNum;
    char lastChar;
    QByteArray readBuffer;
    QString username;
}UserClientInfo;


class Network : public QObject
{
    Q_OBJECT
private:
    QTcpServer *server;
    QList<UserClientInfo *> clientList;

    explicit Network(QObject *parent = nullptr);

protected:
    // 根据id获取clientIndex
    // @param id: client id
    int getUserIndex(int id);

    // 根据username获取clientIndex
    // @param username: client username
    int getUserIndex(QString username);

    // 根据socket获取client index
    // @param s: client socket
    int getUserIndex(QTcpSocket *s);

    // client计数器加1，并返回id；
    static int getUserId();

public:
    static Network *getObject();

    // 根据id获取client username
    // @param id: client id
    QString getUsername(int id);

    // 根据id设置client username
    // @param id: client id
    // @param username: client username
    void setUsername(int id, QString username);

    // 获取用户列表
    QStringList getUsernameList();

    // 启动服务器，监听端口
    // @param port: 服务器准备监听的端口号
    bool startServer(quint16 port);

    // 向client发送数据
    // @param obj: 待发送数据
    // @param id: client username
    bool sendJsonToClient(const QJsonObject &obj, int id);

    // 向client发送数据
    // @param obj: 待发送数据
    // @param username: client username
    bool sendJsonToClient(const QJsonObject &obj, QString username);

protected slots:
    void readyReadSlot();

    void newConnectionSlot();

    void disconnectedSlot();

signals:
    void readJsonFromClient(const QJsonObject &data, int id);
};

#endif // NETWORK_H
