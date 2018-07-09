#ifndef CHLIT_SCREEN_H
#define CHLIT_SCREEN_H

#include <QWidget>
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
#include <QMouseEvent>

class client_screen : public QWidget
{
    Q_OBJECT

public:
    client_screen(QWidget *parent = 0);
    ~client_screen();

    QPoint window_Pos, mousePos, dPos;
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    //void closeEvent(QCloseEvent * );

private:
    QTcpSocket * tcp_client;
    QVBoxLayout * vlayout;
    QHBoxLayout * hlayout;
    QPushButton * button_connect;
    QPushButton * button_savescreen;
    QLabel * label_showscreen;
    QPixmap * pixmap_screen;
    //QByteArray * bytearray;
    QLineEdit * lineedit_address;
    bool is_setlay;
    char * ch_buf;
    int size;
    int read_size;
    int buf_size;
    double screen_size;
    int w, h;

private slots:
    void slot_connect();
    void slot_show();
    void slot_tcperror();
    void slot_change_size();
    void slot_savescreen();
    void slot_change_lay();
};

#endif // CHLIT_SCREEN_H
