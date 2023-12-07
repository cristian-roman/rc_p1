#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "../custom-libraries/myLogger/myLogger.h"
#include "libraries/server_network/server_network.h"

#define RUNNING_STATE 1

void TreatClientRequest();

int main() {
    InitMyLogger();
    InitServerSideNetwork();

    if(NETWORK_OPERATION_STATUS == FAILED)
    {
        exit(EXIT_FAILURE);
    }

    // ReSharper disable once CppDFAEndlessLoop
    while(RUNNING_STATE)
    {
        WaitForClients();
        if(NETWORK_OPERATION_STATUS == FAILED) {
            LogMessage(WARNING_LOG_LEVEL, "Client request denied. Server application will wait for other clients.");
            PrintWarrning("Client request denied. Server application will wait for other clients.");
            continue;
        }

        for (int fd = 0; fd <= MAX_FD; fd++)
        {
            if (fd != SERVER_SOCKET && FD_ISSET(fd, &READ_FDS))
            {
                LogInfoFromPattern("Server connected successfully to a client. Treating client request...");
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, (void *) &TreatClientRequest, NULL);
            }
        }
    }
    //close(server_socket);
    //return 0;
}

void TreatClientRequest()
{
    char* client_message = malloc(256 * sizeof(char));
    const int read_size = read(SERVER_SOCKET, client_message, sizeof(client_message));
    if (read_size < 0)
    {
        LogError("Server application unable to read client request... [REASON] client disconnected.\n");
        PrintWarrning("Server application unable to read client request... [REASON] client disconnected.\n");
        NETWORK_OPERATION_STATUS = FAILED;
        return;
    }

    LogInfoFromPattern("Server application has successfully read client request. Client request: %s", client_message);
    PrintInfoFromPattern("Server application has successfully read client request. Client request: %s", client_message);

    free(client_message);

    char* server_response = malloc(256 * sizeof(char));
    strcpy(server_response, "[Server] Hello client! I have received your request.");
    const int write_size = write(SERVER_SOCKET, server_response, sizeof(server_response));
    if (write_size < 0)
    {
        LogError("Server application unable to write client response... [REASON] client disconnected.\n");
        PrintWarrning("Server application unable to write client response... [REASON] client disconnected.\n");
        NETWORK_OPERATION_STATUS = FAILED;
        return;
    }

    LogInfo("Server application has successfully written client response.");
    PrintInfo("Server application has successfully written client response.");

    free(server_response);
}