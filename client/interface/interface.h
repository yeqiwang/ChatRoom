#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class Interface; }
QT_END_NAMESPACE

class Interface : public QMainWindow
{
    Q_OBJECT

public:
    static Interface * getObject();
    ~Interface();

private slots:
    void on_sendBtn_clicked();

    void reMegSlot(int re);

    void reBindSlot(int re);

    void newMegSlot(const QString &username, const QString &meg);

    void on_bindBtn_clicked();

    void on_meg_returnPressed();

private:
    Interface(QWidget *parent = nullptr);
    Ui::Interface *ui;

    QString myUsername;

    QMessageBox * megbox;
};
#endif // INTERFACE_H
