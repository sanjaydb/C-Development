
// san.cpp
#include "pch.h"
#include "san.h"


#include <iostream>
#include <vector>
#include <windows.h> // Include the Windows API header
#include <fileapi.h>
#include <vector>
#include <functional>
#include <fstream>
#include <assert.h>
#include <lm.h>
#include <sddl.h> //  for ConvertSidToStringSid function 
#include <atlstr.h>
#include <Winsvc.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


extern "C" __declspec(dllexport) bool GetVolumeInfo(const char* driveLetter, char* volumeName, DWORD volumeNameSize, DWORD* serialNumber, DWORD* maxComponentLen, DWORD* fileSystemFlags) {
    // Convert drive letter to wide character (UTF-16)
    wchar_t wideDriveLetter[4] = { L'\0' };
    if (MultiByteToWideChar(CP_UTF8, 0, driveLetter, -1, wideDriveLetter, sizeof(wideDriveLetter)) == 0) {
        return false;
    }

    // Declare wide character variables for volume name and file system name
    wchar_t wideVolumeName[MAX_PATH] = { 0 };
    wchar_t wideFileSystemName[MAX_PATH] = { 0 };

    if (GetVolumeInformationW(wideDriveLetter, wideVolumeName, sizeof(wideVolumeName) / sizeof(wchar_t),
        serialNumber, maxComponentLen, fileSystemFlags,
        wideFileSystemName, sizeof(wideFileSystemName) / sizeof(wchar_t))) {

        // Convert the wide character volume name to a narrow character string
        if (WideCharToMultiByte(CP_UTF8, 0, wideVolumeName, -1, volumeName, volumeNameSize, NULL, NULL) == 0) {
            return false;
        }

        // Check if the volume name is empty (NULL) and set a default name "SAN"
        if (wideVolumeName[0] == L'\0') {
            strncpy_s(volumeName, volumeNameSize, "QRC_NULL", _TRUNCATE);
        }

        return true;
    }
    else {
        return false;
    }
}



extern "C" __declspec(dllexport) int AddNumbers(int a, int b) {
    return a + b;
}



extern "C" __declspec(dllexport) const char* GetHardDriveInfo() {
    // Perform operations to get hard drive information
    const char* hardDriveInfo = "Hard Drive Info: ...";
    return hardDriveInfo;
}


extern "C" __declspec(dllexport) const char* ProcessString(const char* input)
{
    size_t  len = strlen(input);
    char* result = new char[len + 1];

    for (int i = 0; i < len; ++i) {
        result[i] = input[len - i - 1];
    }

    result[len] = '\0';

    return result;
}


extern "C" __declspec(dllexport) UserInformation RPCShareEnum(LPCWSTR rpcIP, LPCWSTR rpcUser, LPCWSTR rpcPassword)
{
    UserInformation p;
    DWORD dwLevel = 0;
    LPUSER_INFO_2 pBuf2 = NULL;

    // Copy the user-specific data
    p.userName = rpcUser;
    p.password = rpcPassword;
    
    // Form the UNC path and assign it to p.url
    size_t bufferSize = wcslen(rpcIP) + wcslen(L"\\\\") + 1;
    wchar_t* uncPath = new wchar_t[bufferSize];
    swprintf(uncPath, bufferSize, L"\\\\%s", rpcIP);
    p.url = new wchar_t[bufferSize];
    wcscpy_s(p.url, bufferSize, uncPath);

    NET_API_STATUS nStatus;
    nStatus = NetUserGetInfo(uncPath, rpcUser, dwLevel, (LPBYTE*)&pBuf2);

    // Assuming pBuf2 contains valid data
    if (nStatus == NERR_Success && pBuf2 != NULL)
    {
        p.userName = pBuf2->usri2_name;
        p.password = pBuf2->usri2_password;
        p.passwd_age = pBuf2->usri2_password_age;
        p.privilege_level = pBuf2->usri2_priv;
       // p.home_directory = pBuf2->usri2_home_dir;
    }
  
    delete[] uncPath;  // Don't forget to clean up allocated memory

    return p;
}

extern "C" __declspec(dllexport) const char** EnumerateRemoteUsers(const wchar_t* ipAddress) {
    LPUSER_INFO_0 pBuf = NULL;
    DWORD dwLevel = 0;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    DWORD dwResumeHandle = 0;
    DWORD i;

    NET_API_STATUS nStatus;

    // Format the server name as "\\IP_Address"
    wchar_t serverName[MAX_PATH];
    swprintf(serverName, MAX_PATH, L"\\\\%s", ipAddress);

    const char** userNameArray = new const char* [1];
    userNameArray[0] = "";

    do {
        nStatus = NetUserEnum(serverName, dwLevel, FILTER_NORMAL_ACCOUNT, (LPBYTE*)&pBuf, dwPrefMaxLen, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);

        if (nStatus == NERR_Success || nStatus == ERROR_MORE_DATA) {
            LPUSER_INFO_0 pTmpBuf = pBuf;

            // Allocate memory for the array of user names
            userNameArray = new

                const char* [dwTotalEntries + 1];

            for (i = 0; i < dwEntriesRead; i++) {
                if (pTmpBuf != NULL) {
                    // Calculate the required buffer size and allocate memory


                    size_t bufferLength = strlen((const char*)pTmpBuf->usri0_name) + 1;
                    userNameArray[i] = (const char*)malloc(bufferLength);

                    // Check if memory allocation was successful


                    if (userNameArray[i] != NULL) 
                    {
                        // Use strcpy_s for safer string copying
                        strcpy_s((char*)userNameArray[i], bufferLength, (const char*)pTmpBuf->usri0_name);
                    }
                    else {
                        // Handle memory allocation failure
                    }
                }
                pTmpBuf++;
            }
        }
        else {
            wprintf(L"A system error has occurred: %d\n", nStatus);
        }

        if (pBuf != NULL) {
            NetApiBufferFree(pBuf);
            pBuf = NULL;
        }
    } while (nStatus == ERROR_MORE_DATA);

    // Ensure that the last element in the array is NULL to indicate the end
    userNameArray[dwTotalEntries] = NULL;

    return userNameArray;
}



extern "C" __declspec(dllexport) int StartServer(void (*callback)(const char* data, const char* error))
{
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
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need the server socket
    closesocket(ListenSocket);

    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Check if the received command is "stop"
            if (strncmp(recvbuf, "stop", 4) == 0) {
                printf("Received 'stop' command. Stopping the server.\n");
                break;
            }

            // Notify the calling code with received data
            if (callback) {
                callback(recvbuf, nullptr); // Pass data, no error
            }

            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                if (callback) {
                    callback(nullptr, "Send failed");
                }
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            if (callback) {
                callback(nullptr, "Receive failed");
            }
            return 1;
        }

    } while (iResult > 0);

    // Shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
    }

    // Cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
