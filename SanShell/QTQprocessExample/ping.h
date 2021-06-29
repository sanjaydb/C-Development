#ifndef PING_H
#define PING_H

#include <QObject>
#include <QDebug>
#include <QSysInfo>
#include <QProcess>


class ping : public QObject
{
    Q_OBJECT
public:
    explicit ping(QObject *parent = nullptr);

    QString operatingSystem();
    QString getAddress() const ;
    void setAddress(const QString &address);



signals:
    void output(QString data);

public slots:
    void start();
    void stop();

private slots:
    void errorOccurred(QProcess::ProcessError error);
    void finished(int exitcode, QProcess::ExitStatus exitStatus);
    void readyReadStandardError();
    void readyReadStandardOutput();
    void started();
    void stateChanged(QProcess::ProcessState newState);
    void readyRead();





private:
    QProcess m_process;
    QString m_address;
    bool m_listening;
    QString getProcess();
    void startPing();


};

#endif // PING_H
