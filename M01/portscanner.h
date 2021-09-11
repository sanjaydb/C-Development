#ifndef PORTSCANNER_H
#define PORTSCANNER_H

#include <QDialog>
#include <QTcpSocket>


namespace Ui {
class PortScanner;
}

class PortScanner : public QDialog
{
    Q_OBJECT

public:
    explicit PortScanner(QWidget *parent = nullptr);
    ~PortScanner();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PortScanner *ui;

    QTcpSocket *mySocket ;

};

#endif // PORTSCANNER_H
