#include <iostream>
#include <windows.h>
#include <winioctl.h>
#include <string>
#include <vector>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")

#define EXPORT extern "C" __declspec(dllexport)

EXPORT std::vector<std::string> GetNetworkInfo()
{
    std::vector<std::string> networkInfo;

    ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    DWORD dwRetVal = 0;

    do
    {
        pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
        if (pAddresses == NULL)
        {
            networkInfo.push_back("Failed to allocate memory for network information.");
            break;
        }

        dwRetVal = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_ALL_INTERFACES, NULL, pAddresses, &outBufLen);
        if (dwRetVal == ERROR_BUFFER_OVERFLOW)
        {
            free(pAddresses);
            pAddresses = NULL;
        }
        else if (dwRetVal == ERROR_SUCCESS)
        {
            networkInfo.push_back("----- Network Information -----");

            PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
            while (pCurrAddresses)
            {
                std::string interfaceName(pCurrAddresses->FriendlyName);
                networkInfo.push_back("Interface: " + interfaceName);

                PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress;
                while (pUnicast)
                {
                    sockaddr* pAddr = pUnicast->Address.lpSockaddr;
                    std::string ipAddress;
                    if (pAddr->sa_family == AF_INET)
                    {
                        sockaddr_in* pInetAddr = reinterpret_cast<sockaddr_in*>(pAddr);
                        ipAddress = std::string(inet_ntoa(pInetAddr->sin_addr));
                    }
                    else if (pAddr->sa_family == AF_INET6)
                    {
                        sockaddr_in6* pInet6Addr = reinterpret_cast<sockaddr_in6*>(pAddr);
                        char strBuffer[INET6_ADDRSTRLEN];
                        inet_ntop(AF_INET6, &(pInet6Addr->sin6_addr), strBuffer, INET6_ADDRSTRLEN);
                        ipAddress = std::string(strBuffer);
                    }

                    if (!ipAddress.empty())
                    {
                        networkInfo.push_back("IP Address: " + ipAddress);
                    }

                    pUnicast = pUnicast->Next;
                }

                std::string macAddress;
                for (ULONG i = 0; i < pCurrAddresses->PhysicalAddressLength; i++)
                {
                    if (i == 0)
                    {
                        macAddress += StringPrintf("%02X", (int)pCurrAddresses->PhysicalAddress[i]);
                    }
                    else
                    {
                        macAddress += StringPrintf(":%02X", (int)pCurrAddresses->PhysicalAddress[i]);
                    }
                }

                if (!macAddress.empty())
                {
                    networkInfo.push_back("MAC Address: " + macAddress);
                }

                networkInfo.push_back("Subnet Mask: " + std::string(pCurrAddresses->Ipv4MaskString));
                
                pCurrAddresses = pCurrAddresses->Next;
            }
        }
        else
        {
            networkInfo.push_back("Failed to retrieve network information.");
        }
    } while (false);

    if (pAddresses != NULL)
    {
        free(pAddresses);
    }

    return networkInfo;
}
