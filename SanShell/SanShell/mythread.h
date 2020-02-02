#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QObject>

class MyThread : public QThread
{
    Q_OBJECT;

public:
    explicit MyThread(QObject *parent = 0);
    void run();
    bool stop ;
    void logFile(QString);

signals:
    void NumberChanged(QString message );


public slots:

};

#endif // MYTHREAD_H
