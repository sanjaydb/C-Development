// -lpcap
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netpacket/packet.h>

#include <net/if.h>
#include <netpacket/packet.h>


#define PORT 12345
#define BUFFER_SIZE 1024
#define LOG_FILE "/var/log/qrcCmd.log"

void logCommand(const char* command) {
    FILE* logFile = fopen(LOG_FILE, "a");
    if (logFile == NULL) {
        perror("Failed to open log file");
        return;
    }

    fprintf(logFile, "\n\nReceived command: %s\n", command);

    fclose(logFile);
}

void logOutput(const char* output) {
    FILE* logFile = fopen(LOG_FILE, "a");
    if (logFile == NULL) {
        perror("\nFailed to open log file");
        return;
    }

    fprintf(logFile, "\nCommand output: %s\n", output);

    fclose(logFile);
}

int isAllowedClient(const struct sockaddr_in* clientAddress) {
    // Check client IP and MAC address here
    return 1;
}

void printIpAddressAndMacAddress() {
    struct ifaddrs* ifaddr, * ifa;
    char ipAddress[INET_ADDRSTRLEN];
    char macAddress[18];

    if (getifaddrs(&ifaddr) == -1) {
        perror("Failed to get network interface information");
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || !(ifa->ifa_flags & IFF_UP))
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in* sa = (struct sockaddr_in*)ifa->ifa_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), ipAddress, INET_ADDRSTRLEN);
        }

        if (ifa->ifa_addr->sa_family == AF_PACKET) {
            struct sockaddr_ll* sa = (struct sockaddr_ll*)ifa->ifa_addr;
            snprintf(macAddress, sizeof(macAddress), "%02X:%02X:%02X:%02X:%02X:%02X",
                     sa->sll_addr[0], sa->sll_addr[1], sa->sll_addr[2],
                     sa->sll_addr[3], sa->sll_addr[4], sa->sll_addr[5]);
        }
    }

    printf("IP Address: %s\n", ipAddress);
    printf("MAC Address: %s\n", macAddress);

    freeifaddrs(ifaddr);
}

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddress, clientAddress;
    int opt = 1;
    int addrlen = sizeof(serverAddress);
    char buffer[BUFFER_SIZE];
    char output[BUFFER_SIZE];

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    printIpAddressAndMacAddress();

    while (1) {
        // Accept a new connection
        if ((newSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), ip, INET_ADDRSTRLEN);

        printf("Connected IP: %s\n", ip);

        // Receive command from client
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(newSocket, buffer, BUFFER_SIZE, 0) <= 0) {
            perror("Receive failed");
            close(newSocket);
            continue;
        }

        printf("\nReceived command: %s\n", buffer);

        // Log received command
        logCommand(buffer);

        // Execute the received command
        FILE* commandOutput = popen(buffer, "r");
        if (commandOutput == NULL) {
            perror("...Failed to execute command");
            strcpy(output, "...Failed to execute command");
        } else {
            memset(output, 0, BUFFER_SIZE);
            fread(output, sizeof(char), BUFFER_SIZE, commandOutput);
            pclose(commandOutput);
        }

	// Check if output is null, blank, or other value
    if (output == NULL || strlen(output) == 0 || strcmp(output, "\n") == 0 || strcmp(output, "") == 0)
 	{
 	       strcpy(output, "QRCSRV :: error executing code");
 	   }
        // Log command output
        logOutput(output);

	printf("\n Data Sent to client ::> %s",output);

        // Send command output to client
        if (send(newSocket, output, strlen(output), 0) <= 0) {
            perror("...Send failed");
        }

        printf("....Command output sent to client \n");

        close(newSocket);
    }

    close(serverSocket);
    return 0;
}

