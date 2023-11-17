
#pragma once

#include "Base.h"


class CSampleService : public CServiceBase
{
public:

    CSampleService(PWSTR pszServiceName,
        BOOL fCanStop = TRUE,
        BOOL fCanShutdown = TRUE,
        BOOL fCanPauseContinue = FALSE);
    virtual ~CSampleService(void);

protected:

    virtual void OnStart(DWORD dwArgc, PWSTR* pszArgv);
    virtual void OnStop();

    void ServiceWorkerThread(void);
    
    void SendToServer(const char* message);
    void HandleServerMessage(const char* message);
    void ConnectToServer();
    void OPenServer();
    void ReceiveMessages();
    void LogToEventLog(const wchar_t* message, WORD eventType);



private:

    BOOL m_fStopping;
    HANDLE m_hStoppedEvent;
};
