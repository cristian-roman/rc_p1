#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "../custom-libraries/myLogger/myLogger.h"
#include "libraries/server_network/server_network.h"

#define RUNNING_STATE 1

void* TreatClientRequest(void* arg);

int main() {
    InitMyLogger();
    InitServerSideNetwork();

    // ReSharper disable once CppDFAEndlessLoop
    while(RUNNING_STATE)
    {
        WaitForClients();
        if(NETWORK_OPERATION_STATUS == FAILED) {
            continue;
        }

        for (int fd = 0; fd <= MAX_FD+1; fd++)
        {
            if (fd != SERVER_SOCKET && FD_ISSET(fd, &READ_FDS))
            {
                LogInfo("Server connected successfully to a client. Treating client request...");

                pthread_t thread_id;
                int cfd = fd;
                pthread_create(&thread_id, NULL, TreatClientRequest, &cfd);
            }
        }
    }
    //close(server_socket);
    //return 0;
}

void* TreatClientRequest(void* arg)
{
    const int client_socket = *(int*)arg;
    char* client_message = malloc(256 * sizeof(char));
    const int read_size = read(client_socket, client_message, 256);
    if (read_size < 0)
    {
        LogError("Server application unable to read client request... [REASON] client abruptly disconnected at server read.\n");
        NETWORK_OPERATION_STATUS = FAILED;
        close(client_socket);
        return (void*)-1;
    }

    LogInfoFromPattern("Server application has successfully read client request. Client request: %s", client_message);
    free(client_message);

    char* server_response = malloc(256 * sizeof(char));
    strcpy(server_response, "[Server] Hello client! I have received your request.\n\0");
    const int write_size = write(client_socket, server_response, strlen(server_response));
    if (write_size < 0)
    {
        LogError("Server application unable to write client response... [REASON] client abrouptly disconnected at server write.\n");
        NETWORK_OPERATION_STATUS = FAILED;
        close(client_socket);
        return (void*)-1;
    }

    LogInfo("Server application has successfully written client response.");

    free(server_response);
    return 0;
}
