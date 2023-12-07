#ifndef SERVER_SERVER_NETWORK_H
#define SERVER_SERVER_NETWORK_H

#define PORT 5050
#define MAX_CLIENTS 5
#include <sys/select.h>

enum OperationStatus
{
    FAILED,
    SUCCEEDED
};

enum OperationStatus NETWORK_OPERATION_STATUS;

int SERVER_SOCKET;
int MAX_FD;
fd_set READ_FDS;
fd_set WRITE_FDS;
struct timeval TIMEOUT;

void CreateServerSocket();

void BindServerSocket();

void OpenPortForListening();

void MultiplexSocket();

void WaitForClients();

void InitServerSideNetwork();

#endif //SERVER_SERVER_NETWORK_H
