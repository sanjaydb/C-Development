#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setText("Os Detcetd : " + m_ping.operatingSystem());
    m_ping.setAddress(ui->lineEdit->text());

    connect(ui->pushButton,&QPushButton::clicked,&m_ping,&ping::start);
    connect(ui->pushButton_2,&QPushButton::clicked,&m_ping,&ping::stop);
    connect(&m_ping,&ping::output,this,&MainWindow::output);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::output(QString data)
{
    ui->plainTextEdit->appendPlainText(data);

}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    m_ping.setAddress(ui->lineEdit->text());

}

