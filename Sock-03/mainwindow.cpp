#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDir>
#include <QDirIterator>
#include <QMessageBox>
//#include <QRegExp>
#include <QApplication>
#include <QObject>






MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mySocket = new QTcpSocket(this);

    connect(mySocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Connect_clicked()
{
    ui->plainTextEdit->clear();
    mySocket->connectToHost("127.0.0.1",9998);
    if(mySocket->waitForConnected(3000)){
        ui->plainTextEdit->appendPlainText("Connected to Server");
    }else{
        ui->plainTextEdit->appendPlainText("Error connecting");
    }
}


void MainWindow::on_pushButton_DisConnect_clicked()
{
    if(mySocket->isOpen()){
        mySocket->close();
        ui->plainTextEdit->appendPlainText("Close");
    }
}

void MainWindow::on_lineEdit_returnPressed()
{
    QFile filex("D:/TMP/d.xlsx");
    //QFile filex("D:/TMP/san01.pptx");

    QByteArray qb;
    qint64  m_size = filex.size() ;

    if (!filex.open(QIODevice::ReadOnly)) return;
    QByteArray blob = filex.readAll();

    qDebug() << blob.size() ;

    if(mySocket->isWritable()){

        //QString myStr = ui->lineEdit->text();
        //memset(myChar,'\0',sizeof(myChar)+1);
        //memcpy(myChar,ui->lineEdit->text().toUtf8(),myStr.length());
        //mySocket->write("PUT /meta HTTP/1.1\r\n");
        //mySocket->write("Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet\r\n");
        //mySocket->write("Content-Type: application/vnd.openxmlformats-officedocument.presentationml.presentation\r\n");


        mySocket->write("PUT /tika/main HTTP/1.1\r\n");
        mySocket->write("Host: localhost:9998\r\n");
        mySocket->write("User-Agent: curl/7.55.1\r\n");
        mySocket->write("Accept: text/plain\r\n");
            QByteArray ff = "Content-Length: "+  qb.setNum(m_size) +"\r\n";
        mySocket->write(ff );
        mySocket->write("Expect: 100-continue\r\n\r\n");
        mySocket->write(blob);
        mySocket->write("\r\n\r\n");
    }
}

void MainWindow::readyRead()
{
    QByteArray myByte ;
        myByte = mySocket->readAll();
    ui->plainTextEdit->appendPlainText(QString (myByte));
    QString b = QString(myByte);
     if (b.contains("Linux",Qt::CaseSensitive))
     {
         qDebug() << "Found";

     }else {
         qDebug() << "Not found";
     }

}

/**
* @brief Luhn algorithm to verify bank card number
* @para cardNum: bank card number
* @author Andy
* @date 2019-03-19
* https://blog.actorsfit.com/a?ID=00950-3466152a-7b82-4127-b6b6-b01499f169b3
*/
bool MainWindow::checkBankCard(QString cardNum){
    int xCheck = cardNum.right(1).toInt();
    cardNum = cardNum.left(cardNum.size()-1);
    int size = cardNum.length();
    if (size> 19) return false;
    int sum = 0;
    for(int n = size; n> 0; n--) {
        int value = cardNum.at(n-1).digitValue();
        if (value == -1) return false;
        if ((size-n)% 2 == 0) {
            value *= 2;
            if(value> 9) value -= 9;
        }
        sum += value;
    }
    return (sum + xCheck)% 10 == 0;
}



void MainWindow::on_pushButton_clicked()
{
//    QVBoxLayout *lay = new QVBoxLayout(this);
//    for(int i=0;i<10;i++)
//    {
//        QCheckBox *dynamic = new QCheckBox("names[i]");
//        dynamic->setChecked (true);
//        lay->addWidget(dynamic);
//    }
//    ui->scrollArea->setLayout(lay);

//    QDir dir;
//    dir.setPath("D:/tmp");

//        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
//        dir.setSorting(QDir::Size | QDir::Reversed);


//        QFileInfoList list = dir.entryInfoList();
//        qDebug() << "     Bytes Filename" ;
//        for (int i = 0; i < list.size(); ++i) {
//            QFileInfo fileInfo = list.at(i);
//            qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
//                                                    .arg(fileInfo.fileName()));
//        }

//    //Create regular expressions
//        QRegExp rxFile("^FILE\\s+(\"[^\"]+\"|\\S+)\\s+(\\w+)$", Qt::CaseInsensitive);
//        QRegExp rxTrack("^TRACK\\s+(\\d+)\\s(\\w+)$", Qt::CaseInsensitive);
//        QRegExp rxIndex("^INDEX\\s+(\\d+)\\s+([0-9:]+)$", Qt::CaseInsensitive);
//        QRegExp rxTitle("^TITLE\\s+(\"[^\"]+\"|\\S+)$", Qt::CaseInsensitive);
//        QRegExp rxPerformer("^PERFORMER\\s+(\"[^\"]+\"|\\S+)$", Qt::CaseInsensitive);
//        QRegExp rxGenre("^REM\\s+GENRE\\s+(\"[^\"]+\"|\\S+)$", Qt::CaseInsensitive);
//        QRegExp rxYear("^REM\\s+DATE\\s+(\\d+)$", Qt::CaseInsensitive);


//        Common credit card vendor regular expressions:

//            Amex Card: ^3[47][0-9]{13}$
//            BCGlobal: ^(6541|6556)[0-9]{12}$
//            Carte Blanche Card: ^389[0-9]{11}$
//            Diners Club Card: ^3(?:0[0-5]|[68][0-9])[0-9]{11}$
//            Discover Card: ^65[4-9][0-9]{13}|64[4-9][0-9]{13}|6011[0-9]{12}|(622(?:12[6-9]|1[3-9][0-9]|[2-8][0-9][0-9]|9[01][0-9]|92[0-5])[0-9]{10})$
//            Insta Payment Card: ^63[7-9][0-9]{13}$
//            JCB Card: ^(?:2131|1800|35\d{3})\d{11}$
//            KoreanLocalCard: ^9[0-9]{15}$
//            Laser Card: ^(6304|6706|6709|6771)[0-9]{12,15}$
//            Maestro Card: ^(5018|5020|5038|6304|6759|6761|6763)[0-9]{8,15}$
//            Mastercard: ^(5[1-5][0-9]{14}|2(22[1-9][0-9]{12}|2[3-9][0-9]{13}|[3-6][0-9]{14}|7[0-1][0-9]{13}|720[0-9]{12}))$
//            Solo Card: ^(6334|6767)[0-9]{12}|(6334|6767)[0-9]{14}|(6334|6767)[0-9]{15}$
//            Switch Card: ^(4903|4905|4911|4936|6333|6759)[0-9]{12}|(4903|4905|4911|4936|6333|6759)[0-9]{14}|(4903|4905|4911|4936|6333|6759)[0-9]{15}|564182[0-9]{10}|564182[0-9]{12}|564182[0-9]{13}|633110[0-9]{10}|633110[0-9]{12}|633110[0-9]{13}$
//            Union Pay Card: ^(62[0-9]{14,17})$
//            Visa Card: ^4[0-9]{12}(?:[0-9]{3})?$
//            Visa Master Card: ^(?:4[0-9]{12}(?:[0-9]{3})?|5[1-5][0-9]{14})$

//                Remove all , and - and other non-digits from the string first.

//                Then use this regex that matches Visa, MasterCard, American Express, Diners Club, Discover, and JCB cards:

//                ^(?:4[0-9]{12}(?:[0-9]{3})?|[25][1-7][0-9]{14}|6(?:011|5[0-9][0-9])[0-9]{12}|3[47][0-9]{13}|3(?:0[0-5]|[68][0-9])[0-9]{11}|(?:2131|1800|35\d{3})\d{11})

//        https://stackoverflow.com/questions/9315647/regex-credit-card-number-tests

 //traverse("D:/TMP/*", ".", 0);

    QDir dir("D:/tmp");

    QDirIterator iterator(dir.absolutePath(), QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        QFile file(iterator.next());
        if ( file.open( QIODevice::ReadOnly ) ){
            qDebug() << "Opened:" << file.fileName();
            ui->plainTextEdit->appendPlainText(file.fileName());
            file.close();
        }else{
            qDebug() << "Can't open " << file.fileName() << file.errorString() ;
    }

}
}



