#include "client_screen.h"

client_screen::client_screen(QWidget *parent)
    : QWidget(parent)
{
    tcp_client = new QTcpSocket(this);//新建一个QTcpSocket
    vlayout = new QVBoxLayout(this);//建立一个vlayout
    hlayout = new QHBoxLayout();//布局
    button_connect = new QPushButton("连接");
    button_savescreen = new QPushButton("截图");
    label_showscreen = new QLabel("无图片");
    pixmap_screen = new QPixmap;
    lineedit_address = new QLineEdit("127.0.0.1");// QLineEdit

    ch_buf = new char[65536*10];//char
    //bytearray = new QByteArray(ch_buf, 65536*10);

    hlayout->addWidget(lineedit_address);
    hlayout->addWidget(button_savescreen);
    hlayout->addWidget(button_connect);

    vlayout->addLayout(hlayout);
    vlayout->addWidget(label_showscreen);

    w = QApplication::desktop()->width();
    h = QApplication::desktop()->height();

    QObject::connect(button_connect, SIGNAL(clicked(bool)),
                     this, SLOT(slot_connect()));
    QObject::connect(tcp_client, SIGNAL(readyRead()),
                     this, SLOT(slot_show()));
    QObject::connect(tcp_client, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(slot_tcperror()));
    QObject::connect(button_savescreen, SIGNAL(clicked(bool)),
                     this, SLOT(slot_savescreen()));
    QObject::connect(lineedit_address, SIGNAL(returnPressed()),
                     this, SLOT(slot_change_lay()));

    size = 0;   //以下是数值的初始化
    buf_size = 0;
    is_setlay = true;
}
//上面这一段是初始化界面，及控件
client_screen::~client_screen()
{
//析构函数
}

void client_screen::mousePressEvent(QMouseEvent *event)
{
    this->window_Pos = label_showscreen->pos();                // 获得部件当前位置
    this->mousePos = event->pos();   // 获得鼠标位置
    this->dPos = mousePos - window_Pos;       // 移动后部件所在的位置

}

void client_screen::mouseMoveEvent(QMouseEvent *event)
{
    label_showscreen->move(event->pos() - this->dPos);
}

/*
void client_screen::closeEvent(QCloseEvent *event)
{
    event->ignore();
}*/

void client_screen::slot_connect()
{
    tcp_client->connectToHost(QHostAddress(lineedit_address->text()), 6655);
}

void client_screen::slot_show()//问题在于传输数据 这个过程
{
    qDebug() << "byte " << tcp_client->bytesAvailable();
    if(size == 0)
    {
        if(tcp_client->read((char*)&size, 4) == -1) return;
        qDebug () << "size" << size;
    }

    buf_size += tcp_client->read(ch_buf+buf_size,
                                       size-buf_size);
    if(buf_size < size)
    {
        return;
    }
    else if(buf_size == size)
    {
        tcp_client->write((char*)&size, 4);
        pixmap_screen->loadFromData((const uchar*)ch_buf, size);
        label_showscreen->setPixmap(*pixmap_screen);

        size = 0;
        buf_size = 0;

        return;
    }
}

void client_screen::slot_tcperror()
{
    tcp_client->reset();
    label_showscreen->setText("连接错误");
}

void client_screen::slot_change_size()
{

}

void client_screen::slot_savescreen()
{
    if(lineedit_address->text() == "")
    {
        slot_change_lay();
    }
    else if(pixmap_screen->save(QString(getenv("HOME"))+"/"+lineedit_address->text()+".jpg", "jpg") == true)
    {
        lineedit_address->setText("ok");
    }
    else
    {
        lineedit_address->setText("error");
    }
}

void client_screen::slot_change_lay()
{
    if(is_setlay == true)
    {
        vlayout->removeWidget(label_showscreen);
        label_showscreen->resize(pixmap_screen->size());
        is_setlay = false;
    }else
    {
        vlayout->addWidget(label_showscreen);
        label_showscreen->resize(pixmap_screen->size());
        is_setlay = true;
    }

}
