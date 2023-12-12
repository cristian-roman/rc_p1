//
// Created by cristian on 12.12.2023.
//

#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H

#define IP_ADDRESS "127.0.0.1"
#define PORT 5050

int ConnectToServer();
void SendMessageToServer(const int client_socket, const char* client_message);
char* ReceiveMessageFromServer(const int client_socket,  const short exepcted_server_response_size);

#endif //CLIENT_NETWORK_H
