#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "custom_c_logger.h"
#include "custom_c_string.h"
#include "client_network.h"
#include "utils.h"

int main(const int argc, char** argv) {

    InitMyLogger(1, STDOUT);

    if(argc != 2) {
        const char* pattern = "Usage:\n%s --single-client\n%s --multiple-clients";
        char* message = GetStringFromPattern(pattern, strlen(pattern) + strlen(argv[0]) * 2 + 10, argv[0], argv[0]);
        LogError(message);
        free(message);
        exit(-1);
    }

    if (strcmp(argv[1], "--single-client") == 0) {
        printf("Please provide two parameters separated by enter(new line):\n- a string representing a valid URL\n- and a positive number representing the depth of the search: \n");

        char* url = ReadURL();

        int depth = ReadDepth();

        while(!IsURLValid(url)) {
            LogError("Url is not valid. Please type a valid url:");
            free(url);
            url = ReadURL();
        }

        while(depth < 1) {
            LogError("Invalid depth. Type a positive number:");
            depth = ReadDepth();
        }

        const char* pattern = "\nInput depth: %d\nInput url: %s\n";
        char* message = GetStringFromPattern(pattern, strlen(pattern) + strlen(url) + 10, depth, url);
        LogInfo(message);
        free(message);

        const int client_socket = ConnectToServer();

        pattern = "%s %d";
        message = GetStringFromPattern(pattern, strlen(pattern) + strlen(url) + 10, url, depth);
        free(url);

        SendMessageToServer(client_socket, message);

        const short expected_server_response_length = 512;
        char* response = ReceiveMessageFromServer(client_socket, expected_server_response_length);

        if(response != NULL) {
            LogInfo(response);
            free(response);
        }

        close(client_socket);
    }

    return 0;
}
