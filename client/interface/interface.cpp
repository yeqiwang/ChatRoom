#include "interface.h"
#include "ui_interface.h"
#include "network/network.h"

#include <QJsonObject>
#include <QDebug>
#include <QDateTime>

Interface::Interface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Interface)
{
    ui->setupUi(this);

    megbox = new QMessageBox;
    megbox->setWindowTitle("提示");

    ui->megs->setReadOnly(true);

    connect(Network::getObject(), SIGNAL(reMeg(int)), this, SLOT(reMegSlot(int)));
    connect(Network::getObject(), SIGNAL(reBind(int)), this, SLOT(reBindSlot(int)));
    connect(Network::getObject(), SIGNAL(newMeg(QString,QString)), this, SLOT(newMegSlot(QString,QString)));

    this->setWindowTitle("Chat Room");
}

Interface *Interface::getObject()
{
    static Interface i;
    return &i;
}

Interface::~Interface()
{
    delete ui;
}

void Interface::on_sendBtn_clicked()
{
    QString username = ui->username_2->text();
    QString meg = ui->meg->text();
    if (myUsername != "") {
        if (meg != ""){
            QJsonObject obj;
            obj.insert("type","meg");
            obj.insert("username", username);
            obj.insert("meg", meg);
            if (!Network::getObject()->sendJsnToServer(obj)){
                megbox->setText("\n消息发送失败，请检查网络是否已连接！\n");
                megbox->exec();
            }
            else {
                QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
                QString clientMeg = myUsername + " " + time +": \n    " + meg + "\n";
                ui->megs->append(clientMeg);
            }
        }
        else {
            megbox->setText("\n消息不能为空！\n");
            megbox->exec();
        }
    }
    else {
        megbox->setText("\n请先绑定用户名！\n");
        megbox->exec();
    }
}

void Interface::reMegSlot(int re)
{
    if (!re){
        megbox->setText("\n消息发送失败，用户不存在或不在线！\n");
        megbox->exec();
    }
    else {
        ui->meg->setText("");
    }
}

void Interface::reBindSlot(int re)
{
    if (re) {
        megbox->setText("\n用户名绑定成功！\n");
        megbox->exec();
    }
    else {
        myUsername = "";
        megbox->setText("\n用户名重复！\n");
        megbox->exec();
    }
//    ui->bindBtn->setEnabled(true);
}

void Interface::newMegSlot(const QString &username, const QString &meg)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString newMeg = username + " " + time + ": \n    " + meg + "\n";
    ui->megs->append(newMeg);
}


void Interface::on_bindBtn_clicked()
{
    QString username = ui->username->text();
    if (username == ""){
        megbox->setText("\n用户名不能为空！\n");
        megbox->exec();
    }
    else {
        myUsername = username;
        QJsonObject obj;
        obj.insert("type", "bind");
        obj.insert("username", username);
        Network::getObject()->sendJsnToServer(obj);
//        ui->bindBtn->setEnabled(false);
    }
}


void Interface::on_meg_returnPressed()
{
    on_sendBtn_clicked();
}

