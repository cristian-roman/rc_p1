#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "custom_c_string.h"
#include "custom_c_logger.h"
#include "libraries/server_network/server_network.h"
#include "libraries/client_handler/client_handler.h"

#define RUNNING_STATE 1

void* TreatClientRequest(void* arg);

int main() {
    InitMyLogger(2, "./logs", STDOUT);
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
                const char* pattern = "Server treating request coming from client with file descriptor: %d";
                char* messageFromPattern = GetStringFromPattern(pattern, strlen(pattern) + 10, fd);
                LogInfo(messageFromPattern);
                free(messageFromPattern);

                pthread_t thread_id;
                int* cfd = malloc(sizeof(int));
                *cfd = fd;
                pthread_create(&thread_id, NULL, TreatClientRequest, cfd);
            }
        }

    }
    //close(server_socket);
    //return 0;
}