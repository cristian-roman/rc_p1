//
// Created by cristian on 12.12.2023.
//

#include "client_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../server_network/server_network.h"
#include "../../../custom_libraries/custom_c_string/custom_c_string.h"
#include "../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../web_handler/web_handler.h"
#include "../web_handler/url_table/url_table.h"
#include "../web_handler/url_handler/url_handler.h"

char* ReadFromClient(const int client_socket, const int expected_message_length) {
    char* client_message = calloc(expected_message_length, sizeof(char));
    const int read_size = read(client_socket, client_message, expected_message_length);
    if (read_size < 0) {
        NETWORK_OPERATION_STATUS = FAILED;
        free(client_message);

        const char* pattern =
                "[Server<-client:%d] unable to read client message... [REASON] client abruptly disconnected";
        char* message = calloc(strlen(pattern) + 10, sizeof(char));
        sprintf(message, pattern, client_socket);
        LogError(message);
        free(message);
        close(client_socket);
        return NULL;
    }

    const char* pattern = "[Server<-client:%d] successfully read client message. Client message: %s";
    char* message = calloc(strlen(pattern) + strlen(client_message) + 10, sizeof(char));
    sprintf(message, pattern, client_socket, client_message);
    LogInfo(message);
    free(message);
    return client_message;
}

int WriteToClient(const int client_socket, const char* server_message) {
    const int write_size = write(client_socket, server_message, strlen(server_message));
    if (write_size < 0) {
        NETWORK_OPERATION_STATUS = FAILED;
        const char* pattern = "[Server->client:%d] unable to write client response... [REASON] client abrouptly disconnected";
        char* log_message = calloc(strlen(pattern) + 10, sizeof(char));
        sprintf(log_message, pattern, client_socket);
        LogError(log_message);
        free(log_message);
        close(client_socket);
        return -1;
    }

    const char* pattern = "[Server->client:%d] successfully written response to client. Server message: %s";
    char* log_message = calloc(strlen(pattern) + strlen(server_message) + 10, sizeof(char));
    sprintf(log_message, pattern, client_socket, server_message);
    LogInfo(log_message);
    free(log_message);
    return 0;
}

void ExtractUrlAndDepthFromClientMessage(char* client_message, char** url, int* depth) {
    const char* token = strtok(client_message, " ");
    int i = 0;
    while(token != NULL && i < 2) {
        if(i == 0) {
            const int token_length = strlen(token);
            *url = calloc(token_length + 10, sizeof(char));
            strncpy(*url, token, token_length);
            EnsureNullOverTheBuffer(*url, token_length);
        }
        else {
            *depth = atoi(token);
        }
        token = strtok(NULL, " ");
        i++;
    }
}

// ReSharper disable once CppNotAllPathsReturnValue
void* TreatClientRequest(void* arg) {
    const int client_socket = *(int *) arg;
    free(arg);
    const int client_message_length = 512;
    char* client_message = ReadFromClient(client_socket, client_message_length);
    if (client_message == NULL) {
        return (void*) -1;
    }

    int relative_depth;
    char *url;
    ExtractUrlAndDepthFromClientMessage(client_message, &url, &relative_depth);
    free(client_message);

    const char* server_response = "Hello client! I have received the url and the depth. Trying to perform resources download...";
    if (WriteToClient(client_socket, server_response) < 0) {
        return (void*) -1;
    }

    const int depth = GetUrlDepth(url);
    const int max_depth = depth + relative_depth;
    struct UrlTable* url_table = InitUrlTable(url, depth, max_depth);
    DumpUrlTable(url_table, 0, max_depth);
    FreeUrlTable(url_table);

    return 0;
}
