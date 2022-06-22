#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>

class Network : public QObject
{
    Q_OBJECT
private:
    explicit Network(QObject *parent = nullptr);
    QTcpSocket *socket;
    qint8 lastChar;
    qint16 braNum;
    QByteArray readBuffer;

public:
    static Network* getObject();

    //连接服务器
    //@param ipAddr 服务器ip
    //@param port 服务器端口号
    void connectToServer(QString ipAddr, quint16 port);

    //向服务器发送数据
    //@param data 要发送的数据
    //@return bool 发送的结果
    bool sendJsnToServer(const QJsonObject &obj);


protected slots:
    void socketReadyReadSlot();

    void readJsonFromServerSlot(const QJsonObject &obj);


signals:
    void readJsonFromServer(const QJsonObject &obj);

    void reMeg(int re);

    void reBind(int re);

    void newMeg(const QString &username, const QString &meg);

};

#endif // NETWORK_H
