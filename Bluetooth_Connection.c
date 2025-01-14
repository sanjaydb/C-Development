#include <winsock2.h>
#include <ws2bth.h>
#include <bluetoothapis.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Bthprops.lib")

int main() {
    WSADATA WSAData = {0};
    SOCKET btSocket = INVALID_SOCKET;
    SOCKADDR_BTH sa = {0};
    ULONG address = 0;
    int status = 0;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    // Create a Bluetooth socket
    btSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    if (btSocket == INVALID_SOCKET) {
        printf("Failed to get a socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Specify the Bluetooth device address and channel
    // Convert the Bluetooth address from string and set the PORT
    sa.addressFamily = AF_BTH;
    sa.btAddr = 0xF7C1476F1033; // Replace with your device's Bluetooth address
    sa.serviceClassId = RFCOMM_PROTOCOL_UUID;
    sa.port = 0; // Typically set to 0 for automatic channel selection, or specify if known

    // Connect to the Bluetooth device
    if (connect(btSocket, (SOCKADDR *)&sa, sizeof(sa)) == SOCKET_ERROR) {
        printf("Connect failed with error: %d\n", WSAGetLastError());
        closesocket(btSocket);
        WSACleanup();
        return 1;
    }

    printf("Connected to device!\n");

    // Connection is established; you can now use send/recv functions
    // Example: send(btSocket, "Hello", 5, 0);

    // Clean up
    closesocket(btSocket);
    WSACleanup();
    return 0;
}
