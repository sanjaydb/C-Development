#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QScreen"
#include "dialog.h"
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>

#include <QMimeType>
#include <QMimeDatabase>
#include <QProcess>
#include <QDirIterator>

#include <QSqlDatabase>

#include <QApplication>
#include <QtWidgets>

#include <QSysInfo>

#include <QSqlDriver>
#include <QSqlQuery>
#include <QMessageBox>
#include <portscanner.h>
#include <QTextStream>



enum { absoluteFileNameRole = Qt::UserRole + 1 };
//! [17]

//! [18]
//static inline QString fileNameOfItem(const QTableWidgetItem *item)
//{
//    return item->data(absoluteFileNameRole).toString();
//}
//! [18]

//! [14]
static inline void openFile(const QString &fileName)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}
//!
//!


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ctime = new QTimer(this);
    connect(ctime,SIGNAL(timeout()),this,SLOT(showTime()));
    ctime->start(1000);

    connect(this, SIGNAL(progressChanged ),
            SLOT(onProgressChanged(QString)));
    move(QGuiApplication::screens().at(0)->geometry().center() - frameGeometry().center());
    // move(screen()->geometry().center() - frameGeometry().center());

    GetLocalIP();



    setWindowTitle(tr("QRC Data Discovery Tool version 1.0"));

    //QPushButton *browseButton = new QPushButton(tr("&Browse..."), this);
    connect(ui->pushButton_2, &QAbstractButton::clicked, this, &MainWindow::browse);


    //findButton = new QPushButton(tr("&Find"), this);
    connect(ui->pushButton_3, &QAbstractButton::clicked, this, &MainWindow::find);

    //fileComboBox = createComboBox(tr("*"));
    //connect(fileComboBox->lineEdit(), &QLineEdit::returnPressed,
    //        this, &MainWindow::animateFindClick);

    //textComboBox = createComboBox();
    //connect(textComboBox->lineEdit(), &QLineEdit::returnPressed,
    //       this, &MainWindow::animateFindClick);

    //directoryComboBox = createComboBox(QDir::toNativeSeparators(QDir::currentPath()));
    //connect(directoryComboBox->lineEdit(), &QLineEdit::returnPressed,
    //      this, &MainWindow::animateFindClick);

    //filesFoundLabel = new QLabel;

    createFilesTable();

    connect(new QShortcut(QKeySequence::Quit, this), &QShortcut::activated,
            qApp, &QApplication::quit);


    connect(ui->ListDirToScan_3, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onListMailItemClicked(QListWidgetItem*)));

    connect(ui->ListDirToScan_4, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onListMailItemClicked4(QListWidgetItem*)));


//QObject::connect(QDirIterator, SIGNAL(signalx(QVariant&)), &r, SLOT(slotx(QVariant&)));




    //qDebug() << QSysInfo::bootUniqueId() ;
    //qDebug() << QSysInfo::buildAbi() ;
    // qDebug() << QSysInfo::buildCpuArchitecture();
    ui->label_16->setText(QSysInfo::currentCpuArchitecture());
    ui->label_27->setText(QSysInfo::kernelType()) ;
    ui->label_28->setText(QSysInfo::machineHostName());
    ui->label_29->setText(QSysInfo::machineUniqueId()) ;
    ui->label_30->setText(QSysInfo::prettyProductName()) ;
    //qDebug() << QSysInfo::productType() ;
    //qDebug() <<  QSysInfo::productVersion() ;

statusBar()->showMessage("[+] QRC CDD Tool Scanning tool");

}




MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::browse()
{
    QString directory =
            QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Find Files"), QDir::currentPath()));
    ui->ListDirToScan->addItem(directory);
}

void MainWindow::find()
{
    //filesTable->setRowCount(0);

    QString fileName = "*.txt";
    //QString text = "378282246310005";
    QString text = ui->lineEdit_2->text();

    QString path = QDir::cleanPath(ui->ListDirToScan->item(0)->text());
    currentDir = QDir(path);
    //! [3]

    //updateComboBox(fileComboBox);
    //updateComboBox(textComboBox);
    //updateComboBox(directoryComboBox);

    //! [4]
    QStringList filter;
    if (!fileName.isEmpty())
        filter << fileName;

    qDebug() <<" Path name  --> " << path ;

    QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QStringList files;
    while (it.hasNext())
        files << it.next(); qDebug() << it.next();
       if (!text.isEmpty())
        files = findFiles(files, text);
    files.sort();
    showFiles(files);
}

void MainWindow::showFiles(const QStringList &paths)
{
    for (const QString &filePath : paths) {
        const QString toolTip = QDir::toNativeSeparators(filePath);
        const QString relativePath = QDir::toNativeSeparators(currentDir.relativeFilePath(filePath));
        const qint64 size = QFileInfo(filePath).size();

        //        QTableWidgetItem *fileNameItem = new QTableWidgetItem(relativePath);
        //        fileNameItem->setData(absoluteFileNameRole, QVariant(filePath));
        //        fileNameItem->setToolTip(toolTip);
        //        fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);

        //        QTableWidgetItem *sizeItem = new QTableWidgetItem(QLocale().formattedDataSize(size));
        //        sizeItem->setData(absoluteFileNameRole, QVariant(filePath));
        //        sizeItem->setToolTip(toolTip);
        //        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        //        sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

        //int row = filesTable->rowCount();
        //filesTable->insertRow(row);
        //filesTable->setItem(row, 0, fileNameItem);
        //filesTable->setItem(row, 1, sizeItem);

        ui->frm_listWidget_DisplayFiles->addItem(relativePath);
    }
    //filesFoundLabel->setText(tr("%n file(s) found (Double click on a file to open it)", nullptr, paths.size()));
    //filesFoundLabel->setWordWrap(true);

    //    QMessageBox msgBox;
    //    msgBox.setText("Searching File Done................!");
    //    msgBox.exec();
}

void MainWindow::animateFindClick()
{
    ui->pushButton_3->animateClick();
}

void MainWindow::openFileOfItem(int row, int column)
{

}

void MainWindow::contextMenu(const QPoint &pos)
{

}







void MainWindow::showTime()
{
    ui->label_18->setText(QTime::currentTime().toString("hh:mm:ss"));
}

QStringList MainWindow::findFiles(const QStringList &files, const QString &text)
{
    QProgressDialog progressDialog(this);
    progressDialog.setCancelButtonText(tr("&Cancel"));
    progressDialog.setRange(0, files.size());
    progressDialog.setWindowTitle(tr("Find Files"));

    //! [5] //! [6]
    QMimeDatabase mimeDatabase;
    QStringList foundFiles;

    for (int i = 0; i < files.size(); ++i) {
        progressDialog.setValue(i);
        progressDialog.setLabelText(tr("Searching file number %1 of %n...", nullptr, files.size()).arg(i));
        QCoreApplication::processEvents();
        //! [6]

        if (progressDialog.wasCanceled())
            break;

        //! [7]
        const QString fileName = files.at(i);
        const QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileName);
        if (mimeType.isValid() && !mimeType.inherits(QStringLiteral("text/plain"))) {
            qWarning() << "Not searching binary file " << QDir::toNativeSeparators(fileName);
            continue;
        }
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QString line;
            QTextStream in(&file);
            while (!in.atEnd()) {
                if (progressDialog.wasCanceled())
                    break;
                line = in.readLine();
                if (line.contains(text, Qt::CaseInsensitive)) {
                    foundFiles << files[i];
                    break;
                }
            }
        }
    }
    return foundFiles;
}


void MainWindow::createFilesTable()
{

}

void MainWindow::GetLocalIP()
{
    GetInternetIP();
    QString s = QDate::currentDate().toString();
    ui->label_19->setText(s);
}

void MainWindow::GetInternetIP()
{
    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53); // google DNS, or something else reliable
    if (socket.waitForConnected()) {
        //qDebug()
        // << "local IPv4 address for Internet connectivity is"
        //<< socket.localAddress();
        QString xip = socket.localAddress().toString();
        ui->label_14->setText(xip);
    }

}





void MainWindow::on_pushButton_6_clicked()
{
    ui->ListDirToScan->clear();
    ui->lineEdit_2->clear();
    ui->frm_listWidget_DisplayFiles->clear();
}





//Databse Run
void MainWindow::on_pushButton_7_clicked()
{
    QString sl = ui->comboBox->currentText();

    if (sl == "SQlite")
    {
        ui->ListDirToScan_2->addItem("CDDToolTestda");
        ui->ListDirToScan_2->addItem("Chinook");
        return ;
    }
    else
    {
        return;
    }
}



void MainWindow::on_pushButton_8_clicked()
{
    a1 = ui->ListDirToScan_2->currentItem()->text() ;


    if (a1 == "CDDToolTestda")
    {
        QSqlDatabase m_db;

        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("D://Development//sqlite-tools-win32-x86-3360000//CDDToolTestda.db");

        if (!m_db.open())
        {
            qDebug() << "Error: connection with database failed";
        }
        else
        {
            qDebug() << "Database: connection ok";

            QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table';");
            //int idName = query.record().indexOf("FirstName");
            while (query.next())
            {
                QString name = query.value(0).toString();
                //qDebug() << name;
                ui->ListDirToScan_3->addItem(name);
            }

        }
    }else
    {
        QSqlDatabase m_db;

        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("D://Development//sqlite-tools-win32-x86-3360000//chinook.db");

        if (!m_db.open())
        {
            qDebug() << "Error: connection with database failed";
        }
        else
        {
            qDebug() << "Database: connection ok";

            QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table';");
            //int idName = query.record().indexOf("FirstName");
            while (query.next())
            {
                QString name = query.value(0).toString();
                //qDebug() << name;
                ui->ListDirToScan_3->addItem(name);
            }

        }
    }
}


void MainWindow::on_pushButton_clicked()
{
    ui->ListDirToScan_2->clear();
    ui->ListDirToScan_3->clear(); ui->ListDirToScan_4->clear();
    ui->ListDirToScan_6->clear();
}


void MainWindow::on_pushButton_9_clicked()
{

    //QMessageBox::information(this,"Push Button 9",a1);

    QSqlDatabase m_db;

    m_db = QSqlDatabase::addDatabase("QSQLITE");

    if (a1 == "Chinook") {
            m_db.setDatabaseName("D://Development//sqlite-tools-win32-x86-3360000//Chinook.db");
    }else{
        m_db.setDatabaseName("D://Development//sqlite-tools-win32-x86-3360000//CDDToolTestda.db");
    }
    if (!m_db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }

}


void MainWindow::onListMailItemClicked(QListWidgetItem* item)
{
    //QMessageBox::information(this,"ListMailClicked",a1);

    qDebug() << item ;
    qDebug() << ui->ListDirToScan_3->currentItem()->text();
    QString SelText =  ui->ListDirToScan_3->currentItem()->text();

    ui->ListDirToScan_4->clear() ;

    QSqlDatabase m_db;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    if (a1 == "Chinook") {
            m_db.setDatabaseName("D://Development//sqlite-tools-win32-x86-3360000//Chinook.db");
    }else{
        m_db.setDatabaseName("D://Development//sqlite-tools-win32-x86-3360000//CDDToolTestda.db");
    }
    if (!m_db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {

        QSqlQuery query("SELECT name FROM pragma_table_info('"+SelText+"');");
        //int idName = query.record().indexOf("FirstName");
        while (query.next())
        {
            QString name = query.value(0).toString();
            qDebug() << name;
            ui->ListDirToScan_4->addItem(name);
        }

    }


}



void MainWindow::onListMailItemClicked4(QListWidgetItem* item)
{
    //QMessageBox::information(this,"ListMailClicked4",a1);

    qDebug() << item ;
    qDebug() << ui->ListDirToScan_4->currentItem()->text();
    QString SelText =  ui->ListDirToScan_4->currentItem()->text();

    ui->ListDirToScan_6->clear() ;

    QSqlDatabase m_db;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    if (a1 == "Chinook") {
            m_db.setDatabaseName("D://Development//sqlite-tools-win32-x86-3360000//Chinook.db");
    }else{
        m_db.setDatabaseName("D://Development//sqlite-tools-win32-x86-3360000//CDDToolTestda.db");
    }
    if (!m_db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {

        qDebug() << "----->" << ui->ListDirToScan_3->currentItem()->text();

        QSqlQuery query("SELECT "+SelText+" from "+ui->ListDirToScan_3->currentItem()->text() + ";");


        //int idName = query.record().indexOf("FirstName");
        while (query.next())
        {
            QString name = query.value(0).toString();
            qDebug() << name;
            ui->ListDirToScan_6->addItem(name);
        }

    }


}

void MainWindow::on_pushButton_10_clicked()
{
    PortScanner p;
    p.setModal(true);
    p.exec();

}


void MainWindow::on_pushButton_5_clicked()
{

    QFile file("D:/Result_storage.txt");
          if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text | QIODevice::ReadWrite))
          {
              QTextStream stream(&file);
                //stream << "Center Point: " << iter_result[0] << "  " << iter_result[1]
                //<< "  " << iter_result[2] << " Rotation: " << iter_result[3] <<'\n';

              int c = ui->frm_listWidget_DisplayFiles->count();
              for (int i = 0; i < c ; ++i){
                        QString s = QString::number(i);
                        QModelIndex *model_index = new QModelIndex(ui->frm_listWidget_DisplayFiles->model()->index(i,0) ); //0th column since we have one cloumn in listwidget
                        QString q= model_index->data(Qt::DisplayRole).toString();
                        stream << q << "\n";
               }

              file.close();
              qDebug() << "Writing finished";
          }

}

