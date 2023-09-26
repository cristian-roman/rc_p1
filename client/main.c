//
// Created by root on 9/9/23.
//


#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "../custom-libraries/myLogger/myLogger.h"

#define PORT 5050

int main() {
    InitMyLogger();

    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        LogError("Client socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with server's IP address

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        LogError("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Now you can send and receive messages with the server using client_socket
    char message[] = "Hello, server!";
    if (send(client_socket, message, strlen(message), 0) == -1) {
        LogError("Sending message failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        LogInfoFromPattern("[Message send to the server]: %s", message);
    }

    close(client_socket);
    return 0;
}
