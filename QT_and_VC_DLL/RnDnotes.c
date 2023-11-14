  std::ofstream logFile("D:\\tmp\\dll_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << "\n\nIP: " << rpcIP << std::endl;
        logFile << "Username: " << rpcUser << std::endl;
        logFile << "Password: " << rpcPassword << std::endl;
        logFile.close();
    }

    if (rpcIP == nullptr || rpcUser == nullptr || rpcPassword == nullptr) {
        // Handle the case where one or more parameters are missing
        result = "Missing parameters";
    }
    else {
        result = "Success"; // Use double quotes for string literals
    }



    --------------------------------
    

LPCWSTR  RPCShareEnum(LPCWSTR  rpcIP, LPCWSTR  rpcUser, LPCWSTR  rpcPassword)
{
    
    const char* result = nullptr; // Initialize result to nullptr

      DWORD dwLevel = 0;
      LPUSER_INFO_0 pBuf = NULL;
      LPUSER_INFO_1 pBuf1 = NULL;
      LPUSER_INFO_2 pBuf2 = NULL;
      LPUSER_INFO_3 pBuf3 = NULL;
      LPUSER_INFO_4 pBuf4 = NULL;
      LPUSER_INFO_10 pBuf10 = NULL;
      LPUSER_INFO_11 pBuf11 = NULL;
      LPUSER_INFO_20 pBuf20 = NULL;
      LPUSER_INFO_23 pBuf23 = NULL;

      NET_API_STATUS nStatus;

      LPTSTR sStringSid = NULL;
    
      int i,j = 0;
    
        //fwprintf_s(stderr, LUsage: % s \\\\ServerName UserName\n, argv[0]);
        //fwprintf_s(stderr, LExample: % s \\\\BosGedik John\n, argv[0]);
        //exit(1);
    
    while (i < 24)
    {
        // Call the NetUserGetInfo function.
        dwLevel = i;
        wprintf(L"\nCalling NetUserGetinfo() with Servername = % s Username = % s Level = % d\n", rpcIP, rpcUser, dwLevel);
        nStatus = NetUserGetInfo(rpcIP, rpcUser, dwLevel, (LPBYTE*)&pBuf);
        // If the call succeeds, print the user information.
        if (nStatus == NERR_Success)
        {
            if (pBuf != NULL)
            {
                switch (i)
                {
                case 0:
                    wprintf(L"\tUser account name : % s\n", pBuf->usri0_name);
                    break;
                case 1:
                    pBuf1 = (LPUSER_INFO_1)pBuf;
                    wprintf(L"\tUser account name : % s\n", pBuf1->usri1_name);
                    wprintf(L"\tPassword: % s\n", pBuf1->usri1_password);
                    wprintf(L"\tPassword age(seconds) : % d\n", pBuf1->usri1_password_age);
                    wprintf(L"\tPrivilege level : % d\n", pBuf1->usri1_priv);
                    wprintf(L"\tHome directory : % s\n", pBuf1->usri1_home_dir);
                    wprintf(L"\tUser comment : % s\n", pBuf1->usri1_comment);
                    wprintf(L"\tFlags(in hex) : % x\n", pBuf1->usri1_flags);
                    wprintf(L"\tScript path : % s\n", pBuf1->usri1_script_path);

                    break;

                case 2:

                    pBuf2 = (LPUSER_INFO_2)pBuf;
                    wprintf(L"\tUser account name : % s\n", pBuf2->usri2_name);
                    wprintf(L"\tPassword: % s\n", pBuf2->usri2_password);
                    wprintf(L"\tPassword age(seconds) : % d\n", pBuf2->usri2_password_age);
                    wprintf(L"\tPrivilege level : % d\n", pBuf2->usri2_priv);
                    wprintf(L"\tHome directory : % s\n", pBuf2->usri2_home_dir);
                    wprintf(L"\tComment: % s\n", pBuf2->usri2_comment);
                    wprintf(L"\tFlags(in hex) : % x\n", pBuf2->usri2_flags);
                    wprintf(L"\tScript path : % s\n", pBuf2->usri2_script_path);
                    wprintf(L"\tAuth flags(in hex) : % x\n", pBuf2->usri2_auth_flags);
                    wprintf(L"\tFull name : % s\n", pBuf2->usri2_full_name);
                    wprintf(L"\tUser comment : % s\n", pBuf2->usri2_usr_comment);
                    wprintf(L"\tParameters: % s\n", pBuf2->usri2_parms);
                    wprintf(L"\tWorkstations: % s\n", pBuf2->usri2_workstations);
                    wprintf(L"\tLast logon(seconds since January 1, 1970 GMT) : % d\n", pBuf2->usri2_last_logon);
                    wprintf(L"\tLast logoff(seconds since January 1, 1970 GMT) : % d\n", pBuf2->usri2_last_logoff);
                    wprintf(L"\tAccount expires(seconds since January 1, 1970 GMT) : % d\n", pBuf2->usri2_acct_expires);
                    wprintf(L"\tMax storage : % d\n", pBuf2->usri2_max_storage);
                    wprintf(L"\tUnits per week : % d\n", pBuf2->usri2_units_per_week);
                    wprintf(L"\tLogon hours : ");

                    for (j = 0; j < 21; j++)
                    {
                        wprintf(L" % x", (BYTE)pBuf2->usri2_logon_hours[j]);
                    }

                    wprintf(L"\n");
                    wprintf(L"\tBad password count : % d\n", pBuf2->usri2_bad_pw_count);
                    wprintf(L"\tNumber of logons : % d\n", pBuf2->usri2_num_logons);
                    wprintf(L"\tLogon server : % s\n", pBuf2->usri2_logon_server);
                    wprintf(L"\tCountry code : % d\n", pBuf2->usri2_country_code);
                    wprintf(L"\tCode page : % d\n", pBuf2->usri2_code_page);

                    break;

                case 4:

                    pBuf4 = (LPUSER_INFO_4)pBuf;

                    wprintf(L"\tUser account name : % s\n", pBuf4->usri4_name);
                    wprintf(L"\tPassword: % s\n", pBuf4->usri4_password);
                    wprintf(L"\tPassword age(seconds) : % d\n", pBuf4->usri4_password_age);
                    wprintf(L"\tPrivilege level : % d\n", pBuf4->usri4_priv);
                    wprintf(L"\tHome directory : % s\n", pBuf4->usri4_home_dir);
                    wprintf(L"\tComment: % s\n", pBuf4->usri4_comment);
                    wprintf(L"\tFlags(in hex) : % x\n", pBuf4->usri4_flags);
                    wprintf(L"\tScript path : % s\n", pBuf4->usri4_script_path);
                    wprintf(L"\tAuth flags(in hex) : % x\n", pBuf4->usri4_auth_flags);
                    wprintf(L"\tFull name : % s\n", pBuf4->usri4_full_name);
                    wprintf(L"\tUser comment : % s\n", pBuf4->usri4_usr_comment);
                    wprintf(L"\tParameters: % s\n", pBuf4->usri4_parms);
                    wprintf(L"\tWorkstations: % s\n", pBuf4->usri4_workstations);
                    wprintf(L"\tLast logon(seconds since January 1, 1970 GMT) : % d\n", pBuf4->usri4_last_logon);
                    wprintf(L"\tLast logoff(seconds since January 1, 1970 GMT) : % d\n", pBuf4->usri4_last_logoff);
                    wprintf(L"\tAccount expires(seconds since January 1, 1970 GMT) : % d\n", pBuf4->usri4_acct_expires);
                    wprintf(L"\tMax storage : % d\n", pBuf4->usri4_max_storage);
                    wprintf(L"\tUnits per week : % d\n", pBuf4->usri4_units_per_week);
                    wprintf(L"\tLogon hours : ");

                    for (j = 0; j < 21; j++)
                    {
                        wprintf(L"% x", (BYTE)pBuf4->usri4_logon_hours[j]);
                    }

                    wprintf(L"\n");
                    wprintf(L"\tBad password count : % d\n", pBuf4->usri4_bad_pw_count);
                    wprintf(L"\tNumber of logons : % d\n", pBuf4->usri4_num_logons);
                    wprintf(L"\tLogon server : % s\n", pBuf4->usri4_logon_server);
                    wprintf(L"\tCountry code : % d\n", pBuf4->usri4_country_code);
                    wprintf(L"\tCode page : % d\n", pBuf4->usri4_code_page);

                    if (ConvertSidToStringSid(pBuf4->usri4_user_sid, &sStringSid))
                    {
                        wprintf(L"\tUser SID : % s\n", sStringSid);
                        LocalFree(sStringSid);
                    }
                    else
                        wprintf(L"ConvertSidToSTringSid() failed with error % d\n", GetLastError());
                    wprintf(L"\tPrimary group ID : % d\n", pBuf4->usri4_primary_group_id);
                    wprintf(L"\tProfile: % s\n", pBuf4->usri4_profile);
                    wprintf(L"\tHome directory drive letter : % s\n", pBuf4->usri4_home_dir_drive);
                    wprintf(L"\tPassword expired information : % d\n", pBuf4->usri4_password_expired);

                    break;

                case 10:

                    pBuf10 = (LPUSER_INFO_10)pBuf;
                    wprintf(L"\tUser account name : % s\n", pBuf10->usri10_name);
                    wprintf(L"\tComment: % s\n", pBuf10->usri10_comment);
                    wprintf(L"\tUser comment : % s\n", pBuf10->usri10_usr_comment);
                    wprintf(L"\tFull name : % s\n", pBuf10->usri10_full_name);

                    break;

                case 11:

                    pBuf11 = (LPUSER_INFO_11)pBuf;

                    wprintf(L"\tUser account name : % s\n", pBuf11->usri11_name);
                    wprintf(L"\tComment: % s\n", pBuf11->usri11_comment);
                    wprintf(L"\tUser comment : % s\n", pBuf11->usri11_usr_comment);
                    wprintf(L"\tFull name : % s\n", pBuf11->usri11_full_name);
                    wprintf(L"\tPrivilege level : % d\n", pBuf11->usri11_priv);
                    wprintf(L"\tAuth flags(in hex) : % x\n", pBuf11->usri11_auth_flags);
                    wprintf(L"\tPassword age(seconds) : % d\n", pBuf11->usri11_password_age);
                    wprintf(L"\tHome directory : % s\n", pBuf11->usri11_home_dir);
                    wprintf(L"\tParameters: % s\n", pBuf11->usri11_parms);
                    wprintf(L"\tLast logon(seconds since January 1, 1970 GMT) : % d\n", pBuf11->usri11_last_logon);
                    wprintf(L"\tLast logoff(seconds since January 1, 1970 GMT) : % d\n", pBuf11->usri11_last_logoff);
                    wprintf(L"\tBad password count : % d\n", pBuf11->usri11_bad_pw_count);
                    wprintf(L"\tNumber of logons : % d\n", pBuf11->usri11_num_logons);
                    wprintf(L"\tLogon server : % s\n", pBuf11->usri11_logon_server);
                    wprintf(L"\tCountry code : % d\n", pBuf11->usri11_country_code);
                    wprintf(L"\tWorkstations: % s\n", pBuf11->usri11_workstations);
                    wprintf(L"\tMax storage : % d\n", pBuf11->usri11_max_storage);
                    wprintf(L"\tUnits per week : % d\n", pBuf11->usri11_units_per_week);
                    wprintf(L"\tLogon hours : ");

                    for (j = 0; j < 21; j++)
                    {
                        wprintf(L" % x", (BYTE)pBuf11->usri11_logon_hours[j]);
                    }

                    wprintf(L"\n");
                    wprintf(L"\tCode page : % d\n", pBuf11->usri11_code_page);

                    break;

                case 20:

                    pBuf20 = (LPUSER_INFO_20)pBuf;

                    wprintf(L"\tUser account name : % s\n", pBuf20->usri20_name);
                    wprintf(L"\tFull name : % s\n", pBuf20->usri20_full_name);
                    wprintf(L"\tComment: % s\n", pBuf20->usri20_comment);
                    wprintf(L"\tFlags(in hex) : % x\n", pBuf20->usri20_flags);
                    wprintf(L"\tUser ID : % u\n", pBuf20->usri20_user_id);

                    break;

                case 23:

                    pBuf23 = (LPUSER_INFO_23)pBuf;

                    wprintf(L"\tUser account name : % s\n", pBuf23->usri23_name);
                    wprintf(L"\tFull name : % s\n", pBuf23->usri23_full_name);
                    wprintf(L"\tComment: % s\n", pBuf23->usri23_comment);
                    wprintf(L"\tFlags(in hex) : % x\n", pBuf23->usri23_flags);

                    if (ConvertSidToStringSid(pBuf23->usri23_user_sid, &sStringSid))
                    {
                        wprintf(L"\tUser SID : % s\n", sStringSid);
                        LocalFree(sStringSid);
                    }
                    else
                        wprintf(L"ConvertSidToSTringSid() failed with error % d\n", GetLastError());
                    break;

                default:
                    break;
                }
            }
        }
        // Otherwise, print the system error.
        else
            fwprintf_s(stderr, L"NetUserGetinfo failed with error : % d\n", nStatus);

        // Free the allocated memory.
        if (pBuf != NULL)
            NetApiBufferFree(pBuf);

        switch (i)
        {
            case 0:
            case 1:
            case 10:
                i++;
                break;
            case 2:
                i = 4;
                break;
            case 4:
                i = 10;
                break;
            case 11:
                i = 20;
                break;
            case 20:
                i = 23;
                break;
            default:
                i = 24;
                break;
        }
    }
    return result;
}

----------------------------

Current New Version Working and used 
------------------------------------
    std::wofstream logFile("D:\\tmp\\dll_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << L"\n\nSAN IP: " << rpcIP << std::endl;
        logFile << L"Username: " << rpcUser << std::endl;
        logFile << L"Password: " << rpcPassword << std::endl;
        logFile.close();
    }

-------- Working Code 20-10-2023
.cpp
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


extern "C" __declspec(dllexport) UserInformation  RPCShareEnum(LPCWSTR  rpcIP, LPCWSTR  rpcUser, LPCWSTR  rpcPassword)
{
    UserInformation p;

    p.password = L"AAAAAAAAAAAAA";
    p.userName = L"BHALERAO";
    
    BYTE logonHoursData[21] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };
    memcpy(p.logonHours, logonHoursData, sizeof(logonHoursData));


    return p;
}



.h 
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
        const wchar_t* userName;  
        const wchar_t* password; 
        BYTE logonHours[21];  
    };




extern "C" {


    __declspec(dllexport) int AddNumbers(int a, int b);
    __declspec(dllexport) const char* GetHardDriveInfo();
    __declspec(dllexport) const char* ProcessString(const char* input);
    __declspec(dllexport) bool GetVolumeInfo(const char* driveLetter, char* volumeName, DWORD volumeNameSize,DWORD* serialNumber, DWORD* maxComponentLen, DWORD* fileSystemFlags);
    __declspec(dllexport) UserInformation  RPCShareEnum(LPCWSTR  rpcIP, LPCWSTR  rpcUser, LPCWSTR  rpcPassword);



}





#ifdef __cplusplus
}
#endif


-x- 



--------


extern "C" __declspec(dllexport) UserInformation  RPCShareEnum(LPCWSTR  rpcIP, LPCWSTR  rpcUser, LPCWSTR  rpcPassword)
{
    UserInformation p;
    DWORD dwLevel = 0;
    LPUSER_INFO_0 pBuf = NULL;
    LPUSER_INFO_2 pBuf2 = NULL;

    std::wofstream logFile("D:\\tmp\\dll_log.txt", std::ios::app);



    //BYTE logonHoursData[21] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };
    //memcpy(p.logonHours, logonHoursData, sizeof(logonHoursData));

    size_t bufferSize = wcslen(rpcIP) + wcslen(L"\\\\") + 1; 
    wchar_t* uncPath = new wchar_t[bufferSize];
    swprintf(uncPath, bufferSize, L"\\\\%s", rpcIP);


    p.password = rpcUser;
    p.userName = rpcPassword;
    p.url = uncPath;


    logFile << L"\n\nInput IP: " << rpcIP << std::endl;
    logFile << L"Input Username: " << rpcUser << std::endl;
    logFile << L"Input Password: " << rpcPassword << std::endl;



    /* NET_API_STATUS nStatus;
    nStatus = NetUserGetInfo(uncPath, rpcUser, dwLevel, (LPBYTE*)&pBuf);

    if (nStatus == NERR_Success)
    {
        if (pBuf != NULL)
        {
            pBuf2 = (LPUSER_INFO_2)pBuf;
            p.userName = pBuf2->usri2_name;
            p.password = pBuf2->usri2_password;
        }
    }
    */

        logFile << L"\n\nOutput IP: " << uncPath << std::endl;
        logFile << L"\n\nOutput Username: " << p.userName << std::endl;
        logFile << L"Output Password: " << p.password << std::endl;

        //logFile << L"\n\nSATTUS: " << nStatus << std::endl;

        //logFile << L"\n\n-------" <<  std::endl;

    //}


    //delete[] uncPath;
    logFile.close();

    return p;
}



