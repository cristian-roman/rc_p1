//
// Created by cristian-roman on 9/7/23.
//

#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include "../custom-libraries/myLogger/myLogger.h"
#include "libraries/server_network/server_network.h"

#define RUNNING_STATE 1

int main() {
    InitMyLogger();
    InitServerSideNetwork();

    if(NETWORK_OPERATION_STATUS == FAILED)
    {
        exit(EXIT_FAILURE);
    }

    while(RUNNING_STATE)
    {
        int client_socket = WaitForClients();
        if(NETWORK_OPERATION_STATUS == FAILED)
        {
            LogInfo("Client request denied. Server application will wait for another clients.");
            continue;
        }
        else
        {
            // Fork to handle the client
            pid_t child_pid = fork();

            if (child_pid == 0) {
                // This is the child process
                close(SERVER_SOCKET); // Child doesn't need to listen

                // Handle communication with the client
                //handle_client(client_socket);
                sleep(3);
                char buffer[1024]; // Buffer to store received message
                ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

                if (bytes_received <= 0) {
                    LogError("Error receiving message");
                } else {
                    buffer[bytes_received] = '\0'; // Add null terminator to make it a string
                    //LogInfoFromPattern("[Received message from client]: %s", buffer);
                }

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
    }
    //close(server_socket);
    //return 0;
}