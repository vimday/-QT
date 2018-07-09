#include "screen.h"

screen::screen(QWidget *parent)
    : QWidget(parent)
{
    vlayout = new QVBoxLayout(this);// QVBoxLayout
    button_getscreen = new QPushButton("开始");// QPushButton按钮
    label_showscreen = new QLabel("无图片");//用label显示图片
    pixmap_screen = new QPixmap(1280, 720);//用qpixmap画布显示图片
    lineedit_ms = new QLineEdit("300");//可编辑的行输入框

    timer = new QTimer(this);//计时器
    timer->setSingleShot(true);//更改为只触发一次

    bytearray = new QByteArray(102400, 0);//字节数组
    buffer = new QBuffer(bytearray, this);//字节流

    tcpserver = new QTcpServer(this);//新建tcp服务器
    tcpclient = new QTcpSocket(this);//新建tcp监听

    tcpserver->listen(QHostAddress::Any, 6655);//设置监听端口

    buffer->open(QIODevice::ReadWrite);//

    screen_this = QGuiApplication::primaryScreen();//小屏幕
    wid_this = QApplication::desktop()->winId();//桌面的小图

    vlayout->addWidget(label_showscreen);//用label布置QVBoxLayout
    vlayout->addWidget(lineedit_ms);//布置可编辑的行输入框
    vlayout->addWidget(button_getscreen);//添加按钮

    setwh();//获取长宽高，然后减一半
    is_connent = 0;//设置标志位，是否连接

    QObject::connect(button_getscreen, SIGNAL(clicked(bool)),
                     this, SLOT(slot_getscreen()));//将按钮与连接的方法，形成信号槽
    QObject::connect(timer, SIGNAL(timeout()), SLOT(slot_send()));//计时器启动
    QObject::connect(tcpserver, SIGNAL(newConnection()), this, SLOT(slot_newconnection()));//tcp的信号
    QObject::connect(lineedit_ms, SIGNAL(returnPressed()), this, SLOT(slot_change_ms()));
}

screen::~screen()
{
    delete bytearray;
}

void screen::setwh()
{
    w = QApplication::desktop()->width()*0.5;//
    h = QApplication::desktop()->height()*0.5;
}

void screen::slot_change_ms()
{
    QString str_tmp;//QT字符对象
    str_tmp = lineedit_ms->text();//获取输入框的字符

    if(str_tmp[0] == '0' || str_tmp[1] == '.')
    {
        size_screen = str_tmp.toDouble();
        w = QApplication::desktop()->width()*size_screen;
        h = QApplication::desktop()->height()*size_screen;
        lineedit_ms->setText("修改屏幕成功");
    }
    else
    {
        msc = str_tmp.toInt();
        lineedit_ms->setText("修改毫秒成功");
    }
}

void screen::slot_getscreen()//截屏处理
{
    *pixmap_screen = screen_this->grabWindow(wid_this);//应该是获取屏幕
    *pixmap_screen = pixmap_screen->scaled(w, h);//屏幕对象按宽高输出图像

    //label_showscreen->setPixmap(*pixmap_screen);//用label显示图像
    pixmap_screen->save(buffer, "jpg");//将图像存入buffer
    size = buffer->pos();
    qDebug() << size << bytearray->size() << w << h;
    buffer->seek(0);
}

void screen::slot_send()//
{
    slot_getscreen();
    qDebug() << "send";
    tcpclient->write((char*)&size, 4);
    tcpclient->write(bytearray->data(), size);
    qDebug() << size << bytearray->size() << w << h;
}

void screen::slot_recv()
{
    tcpclient->read((char*)&recv_size, 4);
    if(recv_size != size)
    {
        qDebug() << recv_size;
        lineedit_ms->setText("传输数据错误");
        tcpclient->reset();
    }
    //qDebug() << "recv";
    timer->start(msc);
}

void screen::slot_newconnection()//新连接处理
{
    if(is_connent)
    {
        lineedit_ms->setText("已有连接");
        return;
    }

    tcpclient = tcpserver->nextPendingConnection();
    QObject::connect(tcpclient, SIGNAL(readyRead()), this, SLOT(slot_recv()));
    QObject::connect(tcpclient, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(slot_error()));
    lineedit_ms->setText("新的连接");
    is_connent = 1;
    timer->start(100);
    qDebug() << "new connection";
}

void screen::slot_error()//出错处理
{
    is_connent = 0;
    lineedit_ms->setText("断开连接");
    tcpclient->reset();
}
