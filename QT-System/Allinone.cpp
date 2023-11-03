.pro 
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui


INCLUDEPATH += D:\Sanjay\Development\WpdPack\Include
LIBS += "-LD:\Sanjay\Development\WpdPack\Lib\x64" -lwpcap -lpacket
LIBS += -lws2_32
LIBS += -L"D:/Sanjay/Development/qrcdll/x64/Debug" -lqrcdll

DEFINES += wpcap
DEFINES += HAVE_REMOTE


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target






.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSystemTrayIcon; // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void xexit();

private slots:
    void showOrHide();

    void on_AppExit_clicked();

    void on_pushButton_clicked();

protected:
    void closeEvent(QCloseEvent *event); // Add this line

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
};
#endif // MAINWINDOW_H


.cpp

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent> // Add this include

#include <QDebug>

#include <pcap.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    trayIcon = new QSystemTrayIcon(this);

    QMenu* trayMenu = new QMenu(this);
    QAction* showAction = trayMenu->addAction("Show");
    trayMenu->addAction("Exit");

    connect(showAction, &QAction::triggered, this, &MainWindow::showOrHide);
    connect(trayMenu->actions().at(1), &QAction::triggered, this, &MainWindow::xexit);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(QIcon("D://Sanjay//pokemon.png"));
    trayIcon->show();

    this->hide(); // Start with the main window hidden
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showOrHide() {
    if (this->isVisible()) {
        this->hide();
    } else {
        this->show();
    }
}

void MainWindow::xexit() {
    trayIcon->hide();
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore(); // Ignore the close event
    this->hide(); // Hide the main window instead of closing
}

void MainWindow::on_AppExit_clicked()
{
    trayIcon->hide();
    QApplication::quit();
}


void MainWindow::on_pushButton_clicked()
{
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int i=0;
    char errbuf[PCAP_ERRBUF_SIZE];

    /* Retrieve the device list from the local machine */
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs_ex: %s\n", errbuf);
        exit(1);
    }

    /* Print the list */
    for(d= alldevs; d != NULL; d= d->next)
    {
        qDebug() <<  d->name;
        if (d->description)
            qDebug() <<  d->description;
        else

            qDebug() << " No description available)\n";
    }

    if (i == 0)
    {
        qDebug() << "\nNo interfaces found! Make sure WinPcap is installed.\n";
        return;
    }

    /* We don't need any more the device list. Free it */
    pcap_freealldevs(alldevs);
}

