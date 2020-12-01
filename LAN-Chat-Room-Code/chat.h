#ifndef CHAT_H
#define CHAT_H

#include <QDialog>

#include<QUdpSocket>
#include<QTcpServer>
#include<QDataStream>
#include<QTime>
#include"tcpserver.h"
#include"tcpclient.h"
#include<QString>
namespace Ui {
class Chat;
}


enum messageType{
    Message,
    NewParticipant,
    LeftParticipant,
    Refuse,
    FileName,
    Xchat


};


class Chat : public QDialog
{
    Q_OBJECT

public:
   // explicit Chat(QWidget *parent = 0);
    ~Chat();
    Chat(QString pasvusername,QString pasvuserip);

    QString xpasuserip;
    QString xpasusername;

    QUdpSocket *xchat;
    qint32 xport;

    void sendMessage(messageType type,QString serverAddress = "");
    quint16 a;

    bool is_opend ;

    static int num1;
    static int num2;


protected:
    void hasPendinFile(QString userName,QString serverAddress,
                       QString clientAddress,QString filename);
    void userLeft(QString userName,QString localHostName,QString time);
    void saveFile(QString fileName);

    bool eventFilter(QObject *target, QEvent *event);

    void closeEvent(QCloseEvent *);

    //void userNameAndTime(QString name,QString time);



private slots:

    void processPendinDatagrams();
    void getFileName(QString);

    void on_boldToolButton_clicked(bool checked);

    void on_italicToolButton_clicked(bool checked);

    void on_underToolButton_clicked(bool checked);

    void on_colorToolButton_clicked();

    void on_sendToolButton_clicked();

    void on_saveToolButton_clicked();

    void on_clearToolButton_clicked();

    void on_closePushButton_clicked();

    void on_sendPushButton_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::Chat *ui;
    TcpServer *server;
    QColor color;
    bool savaFile(const QString fileName);

    QString getMessage();
    QString getIp();
    QString getUserName();


    bool used;

    QString message;
    QString fileName;




};

#endif // CHAT_H
