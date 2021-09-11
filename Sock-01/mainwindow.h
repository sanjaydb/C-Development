#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void onNewConnection();
     void onSocketStateChanged(QAbstractSocket::SocketState socketState);
     void onReadyRead();


private:
    Ui::MainWindow *ui;
    QTcpServer  _server;
       QList<QTcpSocket*>  _sockets;

};
#endif // MAINWINDOW_H
