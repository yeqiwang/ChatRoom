#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <QObject>

class DataHandle : public QObject
{
    Q_OBJECT
private:
    explicit DataHandle(QObject *parent = nullptr);

public:
    static DataHandle *getObject();

protected slots:
    void readJsonFromClientSlot(const QJsonObject &obj, int id);

signals:

};

#endif // DATAHANDLE_H
