#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <QDebug>
#include <mythread.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mThread = new MyThread(this);

    connect(mThread,SIGNAL(NumberChanged(QString)),this,SLOT(onNumberChanged(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::runShell(char *cmd)
{
    pid_t childPID = fork();
    int status;

    if (childPID == -1)
    {
        // fork() returns -1 on failure
        qDebug() << "fork() failed.";
        return (-1);
    }
    else if (childPID == 0)
    {
        // fork() returns 0 to the child process
        FILE *fp;
        char buf[1024];

        qDebug() << cmd ;

        fp = popen(cmd, "r");

        if (fp == NULL)
        {
            printf("Error opening pipe!\n");
            return -1;
        }

        while (fgets(buf, 1204, fp) != NULL)
        {
            qDebug() <<" 1::> " << buf ;
        }

        status = pclose(fp);

        if (status == -1)
        {
            /* Error reported by pclose() */
        } else {
            /* Use macros described under wait() to inspect `status' in order
                to determine success/failure of command executed by popen() */
        }

        //qDebug() << "I am the child, my PID is %d\n" <<  getpid();
        //qDebug() << "My parent's PID is %d\n" <<  getppid();
    }
    else
    {
        // fork() returns the PID of the child to the parent
        //wait(&status); // wait for the child process to finish...
        //qDebug() << "I am the parent, my PID is still %d\n" << getpid();
    }
    return (0);
}

void MainWindow::on_pushButton_clicked()
{
    mThread->start();
}

void MainWindow::on_pushButton_2_clicked()
{
    mThread->stop = true ;
}

void MainWindow::onNumberChanged(QString message)
{
    ui->textEdit->append(message);
}
