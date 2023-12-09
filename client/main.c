#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "../custom-libraries/myLogger/myLogger.h"
#include "../custom-libraries/myString/myString.h"

#define IP_ADDRESS "127.0.0.1"
#define PORT 5050
#define NUM_CLIENTS 10
#define MAX_NUMBER_OF_CLIENTS 10

int connect_to_server()
{
    const int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        LogError("Client socket creation failed");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        LogError("Connection failed");
        close(client_socket);
        return -1;
    }

    return client_socket;
}

void read_url(char* url, const int url_size) {
    char c;
    int characters_read = 0;
    bzero(url, url_size);
    while(c != ' ' && c != '\n') {
        c = getchar();
        strncat(url, &c, 1);
        characters_read++;
        if (characters_read == url_size) {
            LogWarning("URL is too long");
            break;
        }
    }

    while(c != '\n') {
        c = getchar();
    }

    url[strcspn(url, "\n")] = '\0';
}

int main(const int argc, char** argv) {

    InitMyLogger();

    if(argc != 2) {
        LogErrorFromPattern("Usage:\n%s --single-client\n%s --multiple-clients", argv[0], argv[0]);
        exit(-1);
    }

    if (strcmp(argv[1], "--single-client") == 0) {
        printf("Please provide two parameters separated by enter(new line):\n- a string representing a valid URL\n- and a positive number representing the depth of the search: \n");

        const int url_size = 256;
        char* url = malloc(url_size);

        read_url(url, url_size);

        int depth;
        if (scanf("%d", &depth) != 1) {
            LogError("Failed to read depth");
            exit(EXIT_FAILURE);
        }
        getchar();
        LogInfoFromPattern("Depth received: %d\n", depth);
        LogInfoFromPattern("URL received: %s\n", url);

        const int client_socket = connect_to_server();

        const int depth_size = 10;
        char* depthAsString = malloc(depth_size);
        bzero(depthAsString, depth_size);
        IntegerToString(depthAsString, depth);


        const int message_size = 512;
        char* message = malloc(message_size);
        bzero(message, message_size);

        CombineStrings(message, 3, url, " ", depthAsString);
        free(depthAsString);
        free(url);

        int bytes = write(client_socket, message, strlen(message) + 1);
        if (bytes == -1) {
            LogError("Sending message failed");
            close(client_socket);
            free(message);
            exit(EXIT_FAILURE);
        }
        LogInfoFromPattern("[Message sent to the server]: %s", message);
        free(message);

        char server_response[256];
        bytes = read(client_socket, server_response, sizeof(server_response));
        if (bytes == -1) {
            LogError("Receiving message failed");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        LogInfoFromPattern("[Message received from the server]: %s", server_response);

        close(client_socket);
    }

    return 0;
}
