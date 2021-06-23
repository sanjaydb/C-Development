#include "ping.h"
#include <QByteArray>

ping::ping(QObject *parent) : QObject(parent)
{

    connect(&m_process,&QProcess::errorOccurred,this,&ping::errorOccurred);
    connect(&m_process,&QProcess::readyReadStandardError,this,&ping::readyReadStandardError);
    connect(&m_process,&QProcess::readyReadStandardOutput,this,&ping::readyReadStandardOutput);
    connect(&m_process,&QProcess::started,this,&ping::started);
    connect(&m_process,&QProcess::stateChanged,this,&ping::stateChanged);
    connect(&m_process,&QProcess::readyRead,this,&ping::readyRead);

    //finished is overloaded
    connect(&m_process,QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),this,&ping::finished);

    m_listening = false ;
    m_address = "";


}

QString ping::operatingSystem()
{
    return QSysInfo::prettyProductName();

}

QString ping::getAddress() const
{
    return m_address;

}

void ping::setAddress(const QString &address)
{
    m_address = address;

}

void ping::start()
{
    qInfo() << Q_FUNC_INFO;
    m_listening = true;
    m_process.start(getProcess());

}

void ping::stop()
{
    qInfo() << Q_FUNC_INFO;
    m_listening = false;
    m_process.close();

}

void ping::errorOccurred(QProcess::ProcessError error)
{
    if(!m_listening) return;
    qInfo() << Q_FUNC_INFO << error ;
    emit output("Error");


}

void ping::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(!m_listening) return ;
    qInfo() << Q_FUNC_INFO;
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    emit output("Completed");

}

void ping::readyReadStandardError()
{
    if(!m_listening) return;
    qInfo() << Q_FUNC_INFO;
    QByteArray data = m_process.readAllStandardError();
    QString message = "Standard Error :";
    message.append(m_process.readAllStandardError());
    emit output(message);

}

void ping::readyReadStandardOutput()
{
    if(!m_listening) return ;
    qInfo() << Q_FUNC_INFO;
    QByteArray data = m_process.readAllStandardOutput();
    emit output(QString(data.trimmed()));


}

void ping::started()
{
    // DO nothing
    qInfo() << Q_FUNC_INFO;


}

void ping::stateChanged(QProcess::ProcessState newState)
{
    qInfo() << Q_FUNC_INFO ;

    switch(newState) {
    case QProcess::NotRunning:
        emit output("Not Running");
        break;
    case QProcess::Starting:
        emit output("Starting....");
        break;
    case QProcess::Running:
        emit output("Running");
        //start the ping
        startPing();
        break;
    }

}

void ping::readyRead()
{
    if(!m_listening) return;
    qInfo() << Q_FUNC_INFO;
    QByteArray data = m_process.readAll().trimmed();
    qInfo() << data;
    emit output(data);

}

QString ping::getProcess()
{
    qInfo() << Q_FUNC_INFO;
    if(QSysInfo::productType() == "windows") return "cmd";
    if(QSysInfo::productType() == "osx") return "/bin/zsh";

    return "bash";

}

void ping::startPing()
{
    QByteArray command;

    //command.fill(0,80);
    //command.insert(0,ssmd.toLocal8Bit());
    //command.resize(80);
    //command.append(ssmd);


    QString ssmd( "netstat " + m_address) ;
    command.append(ssmd.toUtf8());
    if(QSysInfo::productType() == "windows") command.append("\r");
    command.append("\n");
    m_process.write(command);

}
