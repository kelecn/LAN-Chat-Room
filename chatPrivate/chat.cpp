#include "chat.h"
#include "ui_chat.h"
#include<QHostInfo>
#include<QMessageBox>
#include<QDateTime>
#include<QProcess>
#include<QDataStream>
#include<QScrollBar>
#include<QFont>
#include<QNetworkInterface>
#include<QStringList>
#include<QDebug>
#include<QFileDialog>
#include<QColorDialog>
#include<QHostAddress>
#include<QFontComboBox>
#include<QTextEdit>

int Chat::num1 =0;
int Chat ::num2 = 0;
/*Chat::Chat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chat),
    server(nullptr),
    xchat(nullptr),
    is_opend(false)

{
    ui->setupUi(this);
}
*/
Chat::~Chat()
{
    is_opend = false;
    delete ui;
}



Chat::Chat(QString pasvusername, QString pasvuserip):
    xpasusername(pasvusername),
    xpasuserip(pasvuserip),
    is_opend(false),
    server(nullptr),
    xchat(nullptr),
    used(false),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    ui->messageTextEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textBrowser->setFocusPolicy(Qt::NoFocus);

    ui->messageTextEdit->setFocus();
    ui->messageTextEdit->installEventFilter(this);

    a = 0;
   // is_opend = false;

    ui->label->setText(tr("与%1私聊中 对方的IP：%2").arg(xpasusername)
                       .arg(xpasuserip));

    xchat = new QUdpSocket(this);
    xport = 45456;

    xchat->bind( QHostAddress(getIp()),xport);
    connect(xchat,SIGNAL(readyRead()),
            this,SLOT(processPendinDatagrams()));

    server = new TcpServer(this);
    connect(server, SIGNAL(sendFileName(QString)),
            this,SLOT(getFileName(QString)));




}

void Chat::sendMessage(messageType type, QString serverAddress)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName = QHostInfo::localHostName();
    QString address = getIp();
    out <<type << getUserName() << localHostName;
    switch (type) {
    case LeftParticipant:

        break;

    case Message:
    {
        used = false;
        if(ui->messageTextEdit->toPlainText() =="")
        {
            QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
            return ;
        }
        else
        {
            ui->label->setText(tr("与%1私聊中 对方的IP：%2").arg(xpasusername)
                               .arg(xpasuserip));
            message = getMessage();
            out << address << message;

           // out << address <<getMessage();
            ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
        }
        break;
    }
    case FileName:
    {
        QString clientAddresss = xpasuserip;
        out <<address << clientAddresss << fileName;
        break;
    }
    case Refuse:
    {
        out << serverAddress ;
        break;
    }

    default:
        break;
    }
     qDebug() <<"num1:" << ++num1 << "ipAddress:" << address<<"\n"<<"文件"<<getMessage();
    xchat->writeDatagram(data,data.length(),QHostAddress(xpasuserip),xport);



}

void Chat::processPendinDatagrams()
{
    while (xchat->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(xchat->pendingDatagramSize());
        xchat->readDatagram(datagram.data(),datagram.size());

        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageTyep;
        in >> messageTyep;
        QString userName,localHostName,ipAddress,messagestr;


        qDebug() <<"num2:" << ++num2 << "ipAddress:" << ipAddress;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch (messageTyep) {
        case Xchat:

            break;

        case Message:
        {
            //used = false;

            ui->label->setText(tr("与%1私聊中 对方的IP：%2").arg(xpasusername)
                               .arg(xpasuserip));
            in >> userName >> localHostName >>ipAddress >>messagestr;
            ui->textBrowser->setTextColor(Qt::blue);
            ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
            ui->textBrowser->append("[ " +localHostName+ " ]" + time);
            qDebug() << "messagestr:"<< messagestr;
            ui->textBrowser->append(messagestr);
            this->show();
            is_opend = true;

        }
        case FileName:
        {
            in >> userName >> localHostName >> ipAddress ;
            QString clientAddress,fileName;

            in >> clientAddress >> fileName;

            hasPendinFile(userName,ipAddress,clientAddress,fileName);
            this->show();
            is_opend = true;
            break;

        }
        case Refuse:
        {
            in >>userName >> localHostName;
            QString serverAddress;
            in >>serverAddress;
            if(getIp() == serverAddress)
            {
                server->refused();;
            }
            break;


        }
        case LeftParticipant:
        {
            in >> userName >> localHostName;
            userLeft(userName,localHostName,time);
            //s1 = userName,s2 = time;
           // QMessageBox::information(0,tr("本次对话关闭"),tr("对方结束了对话"),QMessageBox::Ok);
            //ui->textBrowser->clear();
            //ui->messageTextEdit->clear();
            ui->~Chat();
            //close();
            break;
        }
        default:
            break;
        }

    }



}

QString Chat::getMessage()
{
    QString msg = ui->messageTextEdit->toHtml();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return msg;
}




void Chat::getFileName(QString name)
{
    fileName = name;
    sendMessage(FileName);
}


bool Chat::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->messageTextEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *k = static_cast<QKeyEvent *> (event);
            if(k->key() == Qt::Key_Return)
            {

                on_sendPushButton_clicked();
                return true;
            }


        }


    }
    return QWidget::eventFilter(target,event);

}


void Chat :: userLeft(QString userName, QString localHostName, QString time)
{
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    if(!used)
    {
         ui->textBrowser->append(tr("%1于%2离开!").arg(userName).arg(time));
         used = true;
    }

    ui->label->setText(tr("用户%1离开会话界面!").arg(userName));

}

QString Chat::getUserName()
{

    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 <<"HOSTNAME.*" << "DOMAINNAME.*";
    //qDebug() << "envVariables:" << envVariables;
    QStringList environment = QProcess::systemEnvironment();
    //qDebug() << "environment:" << environment;
    foreach (QString string, envVariables) {
        int index = environment.indexOf(QRegExp(string));
        //qDebug() << "index:" <<index;
        if(index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if(stringList.size() == 2)
            {
                return stringList.at(1);
                break;
            }
        }
    }
    return "unknow";

}

QString Chat::getIp()
{
    QList<QHostAddress > list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();

    }
    return 0;
}

void Chat::hasPendinFile(QString userName, QString serverAddress, QString clientAddress, QString fileName)
{
    QString  ipAddress = getIp();
    if(clientAddress  == ipAddress)
    {
        int btn = QMessageBox::information(this,tr("接收文件"),
                                           tr("来自 %1 (%2)的文件:%3","是否接受")
                                           .arg(userName)
                                           .arg(serverAddress).arg(fileName),
                                           QMessageBox::Yes,QMessageBox::No);
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileName);
            if(!name.isEmpty())
            {
                TcpClient *client = new TcpClient(this);
                client->setFileName(name);
                client->setHostAddress(QHostAddress(serverAddress));
                client->show();
                qDebug() << "客户端已创建与服务端的链接" ;

            }
        }
        else if(btn == QMessageBox::No){
            sendMessage(Refuse,serverAddress);
            qDebug() << "severAddress:"<<serverAddress;
        }
}
}




void Chat::on_boldToolButton_clicked(bool checked)
{

    if(checked)
        ui->messageTextEdit->setFontWeight(QFont::Bold);

    else
        ui->messageTextEdit->setFontWeight(QFont::Normal);
    ui->messageTextEdit->setFocus();
}

void Chat::on_italicToolButton_clicked(bool checked)
{
    ui->messageTextEdit->setFontItalic(checked);
    ui->messageTextEdit->setFocus();

}

void Chat::on_underToolButton_clicked(bool checked)
{
    ui->messageTextEdit->setFontUnderline(checked);
    ui->messageTextEdit->setFocus();

}

void Chat::on_colorToolButton_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->messageTextEdit->setTextColor(color);
        ui->messageTextEdit->setFocus();
    }

}

void Chat::on_sendToolButton_clicked()
{

    server->show();
    server->initServer();

}

void Chat::on_saveToolButton_clicked()
{

    if(ui->textBrowser->document()->isEmpty())
    {
        QMessageBox::warning(0,tr("警告"),
                             tr("聊天记录为空,无法保存!"),QMessageBox::Ok);
    }
    else
    {
        QString fileName  = QFileDialog::getSaveFileName(this,tr("保存聊天记录"),tr("聊天记录"),
                                                         tr("文本(*.txt);All File(* . *)"));
        if(!fileName.isEmpty())
            saveFile(fileName);
    }
}

void Chat::saveFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("保存文件"),tr("无法保存文件 %1:\n %2")
                             .arg(fileName)
                             .arg(file.errorString()));
        return ;
    }
    QTextStream out(&file);
    out << ui->textBrowser->toPlainText();


}

void Chat::on_clearToolButton_clicked()
{
    ui->textBrowser->clear();

}

void Chat::on_closePushButton_clicked()
{
    sendMessage(LeftParticipant);
    ui->textBrowser->clear();
    ui->messageTextEdit->clear();
    close();
    //ui->textBrowser->append(tr("%1于%2离开!").arg(s1).arg(s2));
    ui->label->setText(tr("与%1私聊中 对方的IP：%2").arg(xpasusername)
                       .arg(xpasuserip));

    ui->~Chat();
}

void Chat::on_sendPushButton_clicked()
{
    sendMessage(Message);
    QString localHostName = QHostInfo::localHostName();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->textBrowser->setTextColor(Qt::blue);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->textBrowser->append("[" +localHostName+" ]" +time);
    ui->textBrowser->append(message);
    qDebug() << "message" <<message;
    qDebug() <<"发送的文件" <<  getMessage() ;


}

void Chat::closeEvent(QCloseEvent *)
{
    on_closePushButton_clicked();
}

void Chat::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->messageTextEdit->setCurrentFont(f);
    ui->messageTextEdit->setFocus();
}

void Chat::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->messageTextEdit->setFontPointSize(arg1.toDouble());
    ui->messageTextEdit->setFocus();
    //ui->messageTextEdit->setFontPointSize(arg1);
}
