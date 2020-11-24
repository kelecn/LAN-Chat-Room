#include "widget.h"
#include <QApplication>
#include<QTextCodec>
#include<QIcon>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    Widget w;
    w.setWindowIcon(QIcon(":/myicon.ico"));
    w.setWindowTitle("局域网聊天");
    w.show();

    return a.exec();
}
