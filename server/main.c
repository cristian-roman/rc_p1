#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "libraries/server_network/server_network.h"
#include "custom_c_string.h"
#include "libraries/web_handler/web_handler.h"

#define RUNNING_STATE 1

int clientsTreatedCount = 0;

void* TreatClientRequest(void* arg);

int main() {
    InitMyLogger(2, "./logs", STDOUT);
    InitServerSideNetwork();
}

//     time_t start_time = time(NULL);
//
//     // ReSharper disable once CppDFAEndlessLoop
//     while(RUNNING_STATE)
//     {
//         WaitForClients();
//         if(NETWORK_OPERATION_STATUS == FAILED) {
//             continue;
//         }
//
//         for (int fd = 0; fd <= MAX_FD+1; fd++)
//         {
//             if (fd != SERVER_SOCKET && FD_ISSET(fd, &READ_FDS))
//             {
//                 LogInfoFromPattern("Server treating request coming from client with file descriptor: %d", fd);
//
//                 pthread_t thread_id;
//                 int* cfd = malloc(sizeof(int));
//                 *cfd = fd;
//                 pthread_create(&thread_id, NULL, TreatClientRequest, cfd);
//             }
//         }
//
//         const time_t current_time = time(NULL);
//         if (current_time - start_time >= 5)
//         {
//             LogInfoFromPattern("Server treated %d clients in the last 5 seconds", clientsTreatedCount);
//             start_time = current_time;
//             clientsTreatedCount = 0;
//         }
//     }
//     //close(server_socket);
//     //return 0;
// }
//
// void* TreatClientRequest(void* arg)
// {
//     const int client_socket = *(int*)arg;
//     free(arg);
//
//     const int client_message_length = 512;
//     char* client_message = malloc(client_message_length);
//
//     const int read_size = read(client_socket, client_message, client_message_length);
//     if (read_size < 0)
//     {
//         LogErrorFromPattern("[Server<-client:%d] unable to read client message... [REASON] client abruptly disconnected", client_socket);
//         NETWORK_OPERATION_STATUS = FAILED;
//         close(client_socket);
//         free(client_message);
//         return (void*)-1;
//     }
//     LogInfoFromPattern("[Server<-client:%d] successfully read client message. Client message: %s", client_socket, client_message);
//     char** tokens = SplitString(client_message, ' ', 2);
//     free(client_message);
//
//     const int server_response_length = 512;
//     char* server_response = malloc(server_response_length);
//
//     bzero(server_response, server_response_length);
//     strcpy(server_response, "[Server] Hello client! I have received the url and the depth. Trying to perform resources download...");
//
//     int write_size = write(client_socket, server_response, strlen(server_response));
//     if (write_size < 0)
//     {
//         LogErrorFromPattern("[Server->client:%d] unable to write client response... [REASON] client abrouptly disconnected", client_socket);
//         NETWORK_OPERATION_STATUS = FAILED;
//         free(server_response);
//         close(client_socket);
//         return (void*)-1;
//     }
//
//     LogInfoFromPattern("[Server->client:%d] successfully written response to client.", client_socket);
//     bzero(server_response, server_response_length);
//
//     int depth = atoi(tokens[1]);
//
//     char* url = malloc(strlen(tokens[0]) + 1);
//     strcpy(url, tokens[0]);
//     free(tokens);
//
//     download_resource(url, strlen(url), depth);
//
//     clientsTreatedCount++;
//
//     return 0;
// }
