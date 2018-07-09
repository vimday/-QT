#ifndef SCREEN_H
#define SCREEN_H

#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QByteArray>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QDesktopWidget>
#include <QTimer>
#include <QBuffer>
#include <QFile>
#include <QtNetwork>
#include <QLineEdit>
#include <QScreen>

class screen : public QWidget
{
    Q_OBJECT

public:
    screen(QWidget *parent = 0);
    ~screen();

private:
    QHBoxLayout * hlayout;
    QVBoxLayout * vlayout;
    QPushButton * button_getscreen;
    QLabel * label_showscreen;
    QPixmap * pixmap_screen;
    QLineEdit * lineedit_ms;
    QBuffer * buffer;
    QByteArray *bytearray;
    QTcpServer * tcpserver;
    QTcpSocket * tcpclient;
    QScreen * screen_this;
    WId wid_this;
    QTimer* timer;
    int recv_size;
    int size;
    int msc;
    int w;
    int h;
    double size_screen;
    int is_connent;

    void setwh();

private slots:
    void slot_change_ms();
    void slot_getscreen();
    void slot_send();
    void slot_recv();
    void slot_newconnection();
    void slot_error();
};

#endif // SCREEN_H


