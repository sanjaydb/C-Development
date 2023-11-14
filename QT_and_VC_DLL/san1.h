// san.h
#pragma once



#include <Windows.h>
#include <vector>
#include <string>
#include <functional> // Include the header for std::function

#ifdef __cplusplus
extern "C" {
#endif


    struct UserInformation
    {
        LPCWSTR userName;
        LPCWSTR password;
        wchar_t* url; // Change this to wchar_t* for the URL
        int passwd_age;
        int privilege_level;
        LPCWSTR home_directory;
        BYTE logonHours[21];
    };



extern "C" {


    __declspec(dllexport) int AddNumbers(int a, int b);
    __declspec(dllexport) const char* GetHardDriveInfo();
    __declspec(dllexport) const char* ProcessString(const char* input);
    __declspec(dllexport) bool GetVolumeInfo(const char* driveLetter, char* volumeName, DWORD volumeNameSize,DWORD* serialNumber, DWORD* maxComponentLen, DWORD* fileSystemFlags);
    __declspec(dllexport) UserInformation RPCShareEnum(LPCWSTR rpcIP, LPCWSTR rpcUser, LPCWSTR rpcPassword);
    __declspec(dllexport) const char** EnumerateRemoteUsers(const wchar_t* ipAddress);
    __declspec(dllexport) int StartServer(void (*callback)(const char* data, const char* error));



}





#ifdef __cplusplus
}
#endif

