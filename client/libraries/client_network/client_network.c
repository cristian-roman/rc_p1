//
// Created by cristian on 12.12.2023.
//

#include "client_network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../custom_libraries/custom_c_string/custom_c_string.h"

void HandleConnectionError(short* attempt, short *waiting_time, const char* fail_reason)
{
    const char* pattern = "It might be due to the fact that the maximum number of open files has been reached. Retrying in %d seconds";
    char* partial_message = calloc(strlen(pattern) + 10, sizeof(char));
    sprintf(partial_message, pattern, *waiting_time);
    char* message = CombineStrings(2, strlen(partial_message) + strlen(fail_reason) + 10, fail_reason, partial_message);
    free(partial_message);
    LogWarning(message);
    free(message);
    sleep(*waiting_time);
    *attempt += 1;
    *waiting_time *= 2;
}

int ConnectToServer()
{
    const short max_retries = 5;
    const short initial_wait_seconds = 2;

    int client_socket = -1;
    short attempt = 0;
    short wait_time = initial_wait_seconds;

    while (attempt < max_retries) {
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            HandleConnectionError(&attempt, &wait_time, "Socket creation failed.\n");
            continue;
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

        if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
            const char* pattern = "Client socket created successfully. Connected to server with IP address: %s and port: %d";
            char* message = calloc(strlen(pattern) + strlen(IP_ADDRESS) + 10, sizeof(char));
            sprintf(message, pattern, IP_ADDRESS, PORT);
            LogInfo(message);
            free(message);
            return client_socket;
        }

        HandleConnectionError(&attempt, &wait_time, "Connection with the server failed.\n");
    }

    // All attempts failed
    LogError("Exceeded maximum number of connection attempts. Exiting...");
    exit(EXIT_FAILURE);
}

void SendMessageToServer(const int client_socket, const char* client_message)
{
    const int written_size = send(client_socket, client_message, strlen(client_message) + 1, 0);
    if (written_size == -1) {
        const char* pattern = "Failed to send message: %s to server. Consider restarting the client application";
        char* message = calloc(strlen(pattern) + strlen(client_message) + 10, sizeof(char));
        sprintf(message, pattern, client_message);
        LogError(message);
        free(message);
        return;
    }

    const char* pattern = "Message: %s sent successfully to server";
    char* message = calloc(strlen(pattern) + strlen(client_message) + 10, sizeof(char));
    sprintf(message, pattern, client_message);
    LogInfo(message);
    free(message);
}

char* ReceiveMessageFromServer(const int client_socket, const short exepcted_server_response_size)
{
    char* server_response = malloc(exepcted_server_response_size);
    bzero(server_response, exepcted_server_response_size);

    const int read_size = read(client_socket, server_response, exepcted_server_response_size);
    if (read_size == -1) {
        LogError("Failed to read message from server. Consider restarting the client application");
        return NULL;
    }

    const char* pattern = "Message: %s received successfully from server";
    char* message = calloc(strlen(pattern) + strlen(server_response) + 10, sizeof(char));
    sprintf(message, pattern, server_response);
    LogInfo(message);
    return message;
}
