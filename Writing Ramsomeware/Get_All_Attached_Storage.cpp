#include "pch.h"
#include "DLL1.h"
#include <windows.h>
#include <winioctl.h>
#include <string>
#include <vector>

#define EXPORT extern "C" __declspec(dllexport)

std::string GetSizeString(ULONGLONG size)
{
    const double GB = 1024 * 1024 * 1024;
    const double TB = 1024 * 1024 * 1024 * 1024;

    std::string sizeStr;

    if (size >= TB)
    {
        sizeStr = std::to_string(size / TB) + " TB";
    }
    else if (size >= GB)
    {
        sizeStr = std::to_string(size / GB) + " GB";
    }
    else
    {
        sizeStr = std::to_string(size / (1024 * 1024)) + " MB";
    }

    return sizeStr;
}

EXPORT bool GetStorageDeviceInfo(StorageDeviceInfo* info, int* count)
{
    std::vector<StorageDeviceInfo> devices;

    DWORD drives = GetLogicalDrives();
    DWORD mask = 1;

    for (char letter = 'A'; letter <= 'Z'; ++letter, mask <<= 1)
    {
        if (drives & mask)
        {
            std::string drivePath = std::string(1, letter) + ":\\";
            HANDLE hDevice = CreateFileA(drivePath.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

            if (hDevice != INVALID_HANDLE_VALUE)
            {
                ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

                if (GetDiskFreeSpaceExA(drivePath.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
                {
                    StorageDeviceInfo device;
                    device.driveLetter = letter;
                    device.availableSpace = freeBytesAvailable.QuadPart;
                    device.totalSpace = totalNumberOfBytes.QuadPart;
                    device.freeSpace = totalNumberOfFreeBytes.QuadPart;

                    DWORD dwBytesReturned;
                    STORAGE_PROPERTY_QUERY storagePropertyQuery;
                    storagePropertyQuery.PropertyId = StorageDeviceProperty;
                    storagePropertyQuery.QueryType = PropertyStandardQuery;

                    STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = { 0 };
                    if (DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(storagePropertyQuery), &storageDescriptorHeader, sizeof(storageDescriptorHeader), &dwBytesReturned, NULL))
                    {
                        DWORD dwOutBufferSize = storageDescriptorHeader.Size;
                        BYTE* pOutBuffer = new BYTE[dwOutBufferSize];
                        if (DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(storagePropertyQuery), pOutBuffer, dwOutBufferSize, &dwBytesReturned, NULL))
                        {
                            STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer;
                            device.deviceType = pDeviceDescriptor->DeviceType;

                            if (pDeviceDescriptor->VendorIdOffset > 0)
                            {
                                char* vendorIdPtr = (char*)pDeviceDescriptor + pDeviceDescriptor->VendorIdOffset;
                                strncpy_s(device.vendorId, vendorIdPtr, sizeof(device.vendorId) - 1);
                                device.vendorId[sizeof(device.vendorId) - 1] = '\0';
                            }

                            if (pDeviceDescriptor->ProductIdOffset > 0)
                            {
                                char* productIdPtr = (char*)pDeviceDescriptor + pDeviceDescriptor->ProductIdOffset;
                                strncpy_s(device.productId, productIdPtr, sizeof(device.productId) - 1);
                                device.productId[sizeof(device.productId) - 1] = '\0';
                            }

                            if (pDeviceDescriptor->ProductRevisionOffset > 0)
                            {
                                char* productRevisionPtr = (char*)pDeviceDescriptor + pDeviceDescriptor->ProductRevisionOffset;
                                strncpy_s(device.productRevision, productRevisionPtr, sizeof(device.productRevision) - 1);
                                device.productRevision[sizeof(device.productRevision) - 1] = '\0';
                            }
                        }

                        delete[] pOutBuffer;
                    }

                    devices.push_back(device);
                }

                CloseHandle(hDevice);
            }
        }
    }

    *count = static_cast<int>(devices.size());
    if (info != nullptr && *count > 0)
    {
        std::copy(devices.begin(), devices.end(), info);
    }

    return true;
}
