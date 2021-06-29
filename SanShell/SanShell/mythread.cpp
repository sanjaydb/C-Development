#include "mythread.h"
#include <QtCore>
#include <stdio.h>

MyThread::MyThread(QObject *parent ) :
    QThread(parent)
{

}

void MyThread::run()
{
    QMutex mutex ;
    FILE *fp;
    char line[130];			/* line of data from unix command*/
    char *cmd = "nmap -sS -T4 -P0 127.0.0.1 -p1-65535";

    //QString cmd = ("nmap -sS -T4 -P0 127.0.0.1 -p1-65535");
        //char* ch = cmd.toStdString().c_str();


    mutex.lock();
        if(this->stop)
        {
            //break;
        }

        fp = popen(cmd, "r");		/* Issue the command.		*/

        while ( fgets( line, sizeof line, fp))
        {
            emit NumberChanged(line);
            //logFile(line);
        }
        pclose(fp);
    mutex.unlock();
}

void MyThread::logFile(QString data)
{
QString text = data;
text = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + text;
QFile outFile("LogFile.log");
outFile.open(QIODevice::WriteOnly | QIODevice::Append);
QTextStream textStream(&outFile);
textStream << data << endl;

}
