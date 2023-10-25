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


#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "netapi32.lib")


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
