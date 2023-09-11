//
// Created by root on 9/11/23.
//

#ifndef SERVER_SERVER_NETWORK_H
#define SERVER_SERVER_NETWORK_H

#define PORT 5050
#define MAX_CLIENTS 5

enum OperationStatus
{
    FAILED,
    SUCCEEDED
};

enum OperationStatus NETWORK_OPERATION_STATUS;

int SERVER_SOCKET;

void CreateServerSocket();

void BindServerSocket();

void OpenPortForListening();

int WaitForClients();

void InitServerSideNetwork();

#endif //SERVER_SERVER_NETWORK_H
