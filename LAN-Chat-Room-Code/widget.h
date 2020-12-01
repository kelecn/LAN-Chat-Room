#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QUdpSocket>
#include<QTcpServer>
#include"tcpclient.h"
#include"tcpserver.h"
#include<QColor>
#include<QKeyEvent>
#include"chat.h"
namespace Ui {
class Widget;
}

//枚举变量信息类型,分别为消息,新加入者,用户退出,文件名,拒绝接受文件


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void newParticipant(QString userName,QString localHostName,QString ipAddress);

    void leftParticipant(QString userName,QString localHostName,QString time);

    void sendMessage(messageType type,QString serverAddress = "");

    QString getIP();

    QString getUserName();

    QString getMessage();

    void hasPendingFile(QString userName,QString  serverAddress,
                        QString clientAddress,QString fileName);
    bool saveFile(const QString &fileName);

    void closeEvent(QCloseEvent *e);

   // void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

    void showXchat(QString localHostName,QString ipAddress);


private slots:
    void processPendingDatagrams();

    void getFileName(QString);


    void on_sendPushButton_clicked();

    void on_exitPushButton_clicked();

    void on_sendToolBtn_clicked();

    void on_fontComBox_currentFontChanged(const QFont &f);

    void on_sizeComboBox_currentIndexChanged(const QString &arg1);

    void on_boldToolBtn_clicked(bool checked);

    void on_italicToolBtn_clicked(bool checked);

    void on_underlineToolBtn_clicked(bool checked);

    void on_colorToolBtn_clicked();

    void on_saveToolBtn_clicked();

    void on_clearToolBtn_clicked();

    void on_userTableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::Widget *ui;

    QUdpSocket *udpSocket;
    qint16 port;

    QString fileName;
    TcpServer *server;
    QColor color;

    Chat *privateChat;
    Chat *privateChat1;




    static int count,count2;


};

#endif // WIDGET_H
