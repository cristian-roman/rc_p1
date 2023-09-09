//
// Created by cristian-roman on 9/7/23.
//

#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include "../custom-libraries/myLogger/myLogger.h"

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

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        LogError("Listening failed");
        exit(EXIT_FAILURE);
    }

    LogInfoFromPattern("Server listening on port %d...", PORT);

    while(1)
    {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if(client_socket==-1)
        {
            LogError("Acceptance failed");
        }

        // Fork to handle the client
        pid_t child_pid = fork();

        if (child_pid == 0) {
            // This is the child process
            close(server_socket); // Child doesn't need to listen

            // Handle communication with the client
            //handle_client(client_socket);
            sleep(10);

            // Close client socket and exit child process
            close(client_socket);
            exit(EXIT_SUCCESS);
        } else if (child_pid > 0) {
            // This is the parent process
            close(client_socket); // Parent doesn't need this socket
        } else {
            LogError("Fork failed");
            exit(EXIT_FAILURE);
        }
    }
    //close(server_socket);
    //return 0;
}