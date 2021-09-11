#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_pushButton_Connect_clicked();
    void on_pushButton_DisConnect_clicked();
    void on_lineEdit_returnPressed();
    void readyRead();


    void on_pushButton_clicked();
    bool checkBankCard(QString cardNum);

private:
    Ui::MainWindow *ui;

    QTcpSocket *mySocket ;



};
#endif // MAINWINDOW_H
