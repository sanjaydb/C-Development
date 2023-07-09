EXPORT std::vector<std::string> GetHardwareInfo()
{
    std::vector<std::string> hardwareInfo;

    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    hardwareInfo.push_back("----- Hardware Information -----");
    hardwareInfo.push_back("Processor Architecture: " + std::to_string(systemInfo.wProcessorArchitecture));
    hardwareInfo.push_back("Number of Processors: " + std::to_string(systemInfo.dwNumberOfProcessors));
    hardwareInfo.push_back("Page Size: " + std::to_string(systemInfo.dwPageSize));
    hardwareInfo.push_back("Allocation Granularity: " + std::to_string(systemInfo.dwAllocationGranularity));

    return hardwareInfo;
}

EXPORT std::vector<std::string> GetOSInfo()
{
    std::vector<std::string> osInfo;

    OSVERSIONINFOEX osVersionInfo;
    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx((OSVERSIONINFO*)&osVersionInfo);

    osInfo.push_back("----- Operating System Information -----");
    osInfo.push_back("Major Version: " + std::to_string(osVersionInfo.dwMajorVersion));
    osInfo.push_back("Minor Version: " + std::to_string(osVersionInfo.dwMinorVersion));
    osInfo.push_back("Build Number: " + std::to_string(osVersionInfo.dwBuildNumber));
    osInfo.push_back("Platform ID: " + std::to_string(osVersionInfo.dwPlatformId));
    osInfo.push_back("Service Pack: " + std::string(osVersionInfo.szCSDVersion));

    return osInfo;
}

EXPORT std::vector<std::string> GetNetworkInfo()
{
    std::vector<std::string> networkInfo;

    NET_API_STATUS netStatus;
    LPSERVER_INFO_101 pBuf = NULL;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    netStatus = NetServerEnum(NULL, 101, (LPBYTE*)&pBuf, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, SV_TYPE_SERVER, NULL, NULL);

    if (netStatus == NERR_Success)
    {
        networkInfo.push_back("----- Network Information -----");
        networkInfo.push_back("Number of Servers: " + std::to_string(dwEntriesRead));

        for (DWORD i = 0; i < dwEntriesRead; i++)
        {
            std::wstring serverName = pBuf[i].sv101_name;
            std::string serverNameStr(serverName.begin(), serverName.end());
            networkInfo.push_back("Server Name: " + serverNameStr);
        }
    }
    else
    {
        networkInfo.push_back("Failed to retrieve network information.");
    }

    if (pBuf != NULL)
    {
        NetApiBufferFree(pBuf);
    }

    return networkInfo;
}
