#include <windows.h>
#include <stdio.h>
#include <bthsdpdef.h>
#include <bluetoothapis.h>
#include <bthdef.h>

#pragma comment(lib, "Bthprops.lib")

int main()
{
    BLUETOOTH_DEVICE_INFO deviceInfo;
    deviceInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
    deviceInfo.Address.ullLong = 0xF7C1476F1033; // Replace with your device's Bluetooth address

    BLUETOOTH_DEVICE_SEARCH_PARAMS searchParams = {
        sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
        1,    // return authenticated devices
        0,    // return remembered devices
        1,    // return unknown devices
        1,    // return connected devices
        1,    // issue inquiry
        8,    // timeout multipler (x1.28 seconds)
        NULL  // radio handle
    };

    HANDLE hDeviceFind = BluetoothFindFirstDevice(&searchParams, &deviceInfo);
    if (hDeviceFind == NULL) {
        printf("Failed to find any devices.\n");
        return 1;
    }

    do {
        printf("Trying to pair with %S\n", deviceInfo.szName);

        BLUETOOTH_AUTHENTICATION_CALLBACK_PARAMS params;
        HRESULT hr = BluetoothAuthenticateDeviceEx(NULL, NULL, &deviceInfo, NULL, MITMProtectionRequired);
        if (SUCCEEDED(hr)) {
            printf("Successfully paired with %S\n", deviceInfo.szName);
        } else {
            printf("Failed to pair, error %d\n", GetLastError());
        }

    } while (BluetoothFindNextDevice(hDeviceFind, &deviceInfo));

    BluetoothFindDeviceClose(hDeviceFind);
    return 0;
}
