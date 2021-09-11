#include "portscanner.h"
#include "ui_portscanner.h"
#include <QTcpSocket>
#include <QFile>


PortScanner::PortScanner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PortScanner)
{
    ui->setupUi(this);

    mySocket = new QTcpSocket(this);
    connect(mySocket,SIGNAL(readRead()),this,SLOT(readRead()));

}

PortScanner::~PortScanner()
{
    delete ui;
}

void PortScanner::on_pushButton_clicked()
{
    ui->textEdit->clear();
    mySocket->connectToHost("127.0.0.1",9998);

    if(mySocket->waitForConnected(3000)){
        ui->textEdit->append("Connected");
    }else{
        ui->textEdit->append("Not");
    }

//        QString host = "localhost";
//        QString startPort = "1";
//        quint16 startPortInt = startPort.toUShort();
//        QString endPort = "65535";
//        quint16 endPortInt = endPort.toUShort();
//        QString timeout = "30";
//        quint16 timeoutInt = timeout.toUShort();
//        qInfo() << host;
//        QTcpSocket socket;
//ui->textEdit->append("Scanning Localhost ");

//        for(quint16 i = startPortInt; i < endPortInt; i++){

//                socket.connectToHost(host, i);
//                if(socket.waitForConnected(timeoutInt)){
//                    qInfo() << "Open Port: " << i;
//                    QString openPort = QString::number(i);
//                    ui->textEdit->append("Port: " + openPort);
//                    socket.disconnectFromHost();
//                }
//        }
//        ui->textEdit->append("Scan Complete");
//       qInfo() << "Scan Complete";

//QTcpSocket socket;
//quint16 i = 9998;
//socket.connectToHost("localhost", i);


//socket.waitForConnected(3000);
//QFile file("D:/tmp/f.docx");   //file path
//file.open(QIODevice::ReadOnly);
//QByteArray q = file.readAll();
//socket.write(q);

//qDebug() << socket.readAll();

////HTTP/1.1 100 Continue

////PUT /tika/main HTTP/1.1
////Host: localhost:9998
////User-Agent: curl/7.55.1
////Accept: text/plain
////Content-Length: 536709
////Expect: 100-continue

}

