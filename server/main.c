#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../custom-libraries/myLogger/myLogger.h"
#include "libraries/server_network/server_network.h"

#define RUNNING_STATE 1

int clientsTreatedCount = 0;

void* TreatClientRequest(void* arg);

int main() {
    InitMyLogger();
    InitServerSideNetwork();

    time_t start_time = time(NULL);

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
                LogInfoFromPattern("Server treating request coming from client with file descriptor: %d", fd);

                pthread_t thread_id;
                int* cfd = malloc(sizeof(int));
                *cfd = fd;
                pthread_create(&thread_id, NULL, TreatClientRequest, cfd);
            }
        }

        const time_t current_time = time(NULL);
        if (current_time - start_time >= 5)
        {
            LogInfoFromPattern("Server treated %d clients in the last 5 seconds", clientsTreatedCount);
            start_time = current_time;
            clientsTreatedCount = 0;
        }
    }
    //close(server_socket);
    //return 0;
}

void* TreatClientRequest(void* arg)
{
    const int client_socket = *(int*)arg;
    free(arg);
    char* client_message = malloc(256 * sizeof(char));
    const int read_size = read(client_socket, client_message, 256);
    if (read_size < 0)
    {
        LogErrorFromPattern("[Server<-client:%d] unable to read client request... [REASON] client abruptly disconnected", client_socket);
        NETWORK_OPERATION_STATUS = FAILED;
        close(client_socket);
        return (void*)-1;
    }
    if(strcmp(client_message, "Hello, server!") == 0) {
        clientsTreatedCount++;
    }
    LogInfoFromPattern("[Server<-client:%d] successfully read client request. Client request: %s", client_socket, client_message);
    free(client_message);

    char* server_response = malloc(256 * sizeof(char));
    strcpy(server_response, "[Server] Hello client! I have received your request.\n\0");
    const int write_size = write(client_socket, server_response, strlen(server_response));
    if (write_size < 0)
    {
        LogErrorFromPattern("[Server->client:%d] unable to write client response... [REASON] client abrouptly disconnected", client_socket);
        NETWORK_OPERATION_STATUS = FAILED;
        close(client_socket);
        return (void*)-1;
    }

    LogInfoFromPattern("[Server->client:%d] successfully written response to client.", client_socket);
    free(server_response);

    return 0;
}
