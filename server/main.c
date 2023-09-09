//
// Created by cristian-roman on 9/7/23.
//

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "./custom-libraries/myLogger/myLogger.h"

#define PORT 8080
#define MAX_CLIENTS 5

int main() {
    InitMyLogger();

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    //Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1)
    {
        LogError("Server socket creation failed");
        exit(EXIT_FAILURE);
    }

    //Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) ==1)
    {
        LogError("Binding failed");
        exit(EXIT_FAILURE);
    }

    LogInfoFromPattern("Server listening on port %d...", PORT);

}