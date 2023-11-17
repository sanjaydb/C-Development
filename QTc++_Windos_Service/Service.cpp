
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <string>
#include <wchar.h>
#include <Windows.h>

#include "Service.h"
#include "ThreadPool.h"


#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "netapi32.lib")


//#include <Winsvc.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>

#define DEFAULT_PORT "1890"
#define DEFAULT_SERVER_IP "192.168.1.202"
#define DEFAULT_SERVER_PORT "12345"
#define DEFAULT_BUFLEN 512



SOCKET clientSocket = INVALID_SOCKET;


CSampleService::CSampleService(PWSTR pszServiceName,
    BOOL fCanStop,
    BOOL fCanShutdown,
    BOOL fCanPauseContinue)
    : CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
    m_fStopping = FALSE;

    m_hStoppedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_hStoppedEvent == NULL)
    {
        throw GetLastError();
    }
}


CSampleService::~CSampleService(void)
{
    if (m_hStoppedEvent)
    {
        CloseHandle(m_hStoppedEvent);
        m_hStoppedEvent = NULL;
    }
}



void CSampleService::OnStart(DWORD dwArgc, LPWSTR* lpszArgv)
{
    // Log a service start message to the Application log.
    WriteEventLogEntry((PWSTR)L"QRC-Agent03 in OnStart",EVENTLOG_INFORMATION_TYPE);

    // Queue the main service function for execution in a worker thread.
    CThreadPool::QueueUserWorkItem(&CSampleService::ServiceWorkerThread, this);
}



void CSampleService::OnStop()
{

    WriteEventLogEntry((PWSTR)L"QRC-Agent03 in OnStop", EVENTLOG_INFORMATION_TYPE);


    m_fStopping = TRUE;
    if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
    {
        throw GetLastError();
    }
}



void CSampleService::ServiceWorkerThread(void)
{
    // Periodically check if the service is stopping.
    while (!m_fStopping)
    {
    
        WriteEventLogEntry((PWSTR)L" In Function ServiceWorkerThread :: Before ConnectToServer()   ", EVENTLOG_INFORMATION_TYPE);

        ConnectToServer();

        WriteEventLogEntry((PWSTR)L" In Function ServiceWorkerThread :: After ConnectToServer()   ", EVENTLOG_INFORMATION_TYPE);


        ::Sleep(2000);  // Simulate some lengthy operations.
    }

    SetEvent(m_hStoppedEvent);
}




void CSampleService::SendToServer(const char* message) {

    WriteEventLogEntry((PWSTR)L"::> in SendToServer  :: ", EVENTLOG_INFORMATION_TYPE);

    if (clientSocket != INVALID_SOCKET) {
        send(clientSocket, message, strlen(message), 0);
    }
    WriteEventLogEntry((PWSTR)L"::> Leaving SendToServer  :: ", EVENTLOG_INFORMATION_TYPE);

}

void CSampleService::HandleServerMessage(const char* message) {
    // Process the received message and reply if needed
    if (strcmp(message, "NAME") == 0) {
        // Reply with IP address
        char ipAddress[256];
        gethostname(ipAddress, sizeof(ipAddress));
        SendToServer(ipAddress);
    }
}


void CSampleService::ConnectToServer()
{

    WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 01  :: ", EVENTLOG_INFORMATION_TYPE);
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) 
    {
        //std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 02  :: iResult ", EVENTLOG_INFORMATION_TYPE);

        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT, &hints, &result);
    if (iResult != 0) 
    {

        //std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 03  ::GetAddressInfo Failed  ", EVENTLOG_INFORMATION_TYPE);

        WSACleanup();
        return;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            //std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
            WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 04  ::Socket Failed  ", EVENTLOG_INFORMATION_TYPE);

            WSACleanup();
            return;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        //std::cout << "Unable to connect to server!" << std::endl;
        WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 05  ::Unable to connect Server ", EVENTLOG_INFORMATION_TYPE);

        WSACleanup();
        return;
    }

    // Send an initial buffer
    const char* sendBuffer = "Hello, server!";
    iResult = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
    if (iResult == SOCKET_ERROR) {
        //std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 06  ::Send Fail  ", EVENTLOG_INFORMATION_TYPE);

        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }

    //std::cout << "Bytes Sent: " << iResult << std::endl;
    WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 07  :: Byte Sent  ", EVENTLOG_INFORMATION_TYPE);

    // Receive until the peer closes the connection
    char recvbuf[DEFAULT_BUFLEN];
    do {
        iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0)
            //std::cout << "Bytes received: " << iResult << std::endl;
            WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 08  ::Byte Recived  ", EVENTLOG_INFORMATION_TYPE);

        else if (iResult == 0)
            //std::cout << "Connection closed" << std::endl;
            WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 09  ::Connection closed ", EVENTLOG_INFORMATION_TYPE);

        else
            //std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
            WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 10  ::Recived Failed  ", EVENTLOG_INFORMATION_TYPE);

    } while (iResult > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    WriteEventLogEntry((PWSTR)L"::> Inside ConnectToServer() :: SAN 11  ::Leaving Function  ", EVENTLOG_INFORMATION_TYPE);

}


void CSampleService::OPenServer() 
{
    // Create socket

    WriteEventLogEntry((PWSTR)L"::> Inside OpenServer() .......", EVENTLOG_INFORMATION_TYPE);

    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
       // printf("WSAStartup failed with error: %d\n", iResult);
        WriteEventLogEntry((PWSTR)L"::> OpenServer :: iResult 03 .......", EVENTLOG_INFORMATION_TYPE);

        return ;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    
    if (iResult != 0) 
    {
        //printf("getaddrinfo failed with error: %d\n", iResult);
        WriteEventLogEntry((PWSTR)L"::> OpenServer :: iResult 04 .......", EVENTLOG_INFORMATION_TYPE);

        WSACleanup();
        return ;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    
    if (ListenSocket == INVALID_SOCKET) 
    {
        //printf("socket failed with error: %ld\n", WSAGetLastError());
        WriteEventLogEntry((PWSTR)L"::> OpenServer :: ListenSocket 04 .......", EVENTLOG_INFORMATION_TYPE);

        freeaddrinfo(result);
        WSACleanup();
        return ;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
    {
        //printf("bind failed with error: %d\n", WSAGetLastError());
        WriteEventLogEntry((PWSTR)L"::> OpenServer :: iResult 05 ....Bind ", EVENTLOG_INFORMATION_TYPE);

        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return ;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) 
    {
        //printf("listen failed with error: %d\n", WSAGetLastError());
        WriteEventLogEntry((PWSTR)L"::> OpenServer :: iResult 06 .......", EVENTLOG_INFORMATION_TYPE);

        closesocket(ListenSocket);
        WSACleanup();
        return ;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);

    if (ClientSocket == INVALID_SOCKET) 
    {
       // printf("accept failed with error: %d\n", WSAGetLastError());
        WriteEventLogEntry((PWSTR)L"::> OpenServer :: iResult 07 .......", EVENTLOG_INFORMATION_TYPE);

        closesocket(ListenSocket);
        WSACleanup();
        return ;
    }

    // No longer need the server socket
    closesocket(ListenSocket);

    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            //printf("Bytes received: %d\n", iResult);

            // Check if the received command is "stop"
            if (strncmp(recvbuf, "stop", 4) == 0) 
            {
                WriteEventLogEntry((PWSTR)L"::>OpenServer:: iResult 08 .......", EVENTLOG_INFORMATION_TYPE);

                //printf("Received 'stop' command. Stopping the server.\n");
                break;
            }

            // Notify the calling code with received data
            

            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) 
            {
             //   printf("send failed with error: %d\n", WSAGetLastError());
                WriteEventLogEntry((PWSTR)L"::> OpenServer :: iResult 09 .......", EVENTLOG_INFORMATION_TYPE);

                closesocket(ClientSocket);
                WSACleanup();
                
                return ;
            }


            //printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            //printf("Connection closing...\n");
            WriteEventLogEntry((PWSTR)L"::> OpenServer :: iResult 10 .......", EVENTLOG_INFORMATION_TYPE);

        else {

            WriteEventLogEntry((PWSTR)L"::> OpenServer :: iResult 11 .......", EVENTLOG_INFORMATION_TYPE);

            //printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
         
            return ;
        }

    } while (iResult > 0);

    // Shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
       // printf("shutdown failed with error: %d\n", WSAGetLastError());
        WriteEventLogEntry((PWSTR)L"::> OpenServer :: iResult 12 .......", EVENTLOG_INFORMATION_TYPE);

    }

    // Cleanup
    closesocket(ClientSocket);
    WSACleanup();

    WriteEventLogEntry((PWSTR)L"::> Leaving Server Code 13 .......", EVENTLOG_INFORMATION_TYPE);

    return;
}

void CSampleService::ReceiveMessages() {
    char buffer[256];
    int bytesRead;

    WriteEventLogEntry((PWSTR)L"::> in ReciveMessages  :: ", EVENTLOG_INFORMATION_TYPE);


    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        wchar_t logMessage[256];
        buffer[bytesRead] = '\0';  // Null-terminate the received data


        WriteEventLogEntry((PWSTR)L"::> in ReciveMessages  :: inside while loop", EVENTLOG_INFORMATION_TYPE);

        // Use wcscpy_s for safer string copying
        wcscpy_s(logMessage, L"Received from server: ");

        // Use wcscat_s for safer string concatenation
        wcscat_s(logMessage, (size_t)wcslen(logMessage) + bytesRead + 1, (wchar_t*)buffer);

        LogToEventLog(logMessage, EVENTLOG_INFORMATION_TYPE);
        // Handle the received message
        HandleServerMessage(buffer);
    }

    // Handle disconnect or error
    //LogToEventLog(L"From Resc-Function : Disconnected from server.. ", EVENTLOG_INFORMATION_TYPE);
    WriteEventLogEntry((PWSTR)L"From Resc-Function : Disconnected from server..", EVENTLOG_INFORMATION_TYPE);

    closesocket(clientSocket);
    clientSocket = INVALID_SOCKET;
}


void CSampleService::LogToEventLog(const wchar_t* message, WORD eventType)
{
    WriteEventLogEntry((PWSTR)message, eventType);
}
