#ifndef SERVER_SERVER_NETWORK_H
#define SERVER_SERVER_NETWORK_H

#define PORT 5050
#define MAX_CLIENTS 15
#include <sys/select.h>

enum OperationStatus
{
    FAILED,
    SUCCEEDED
};

extern enum OperationStatus NETWORK_OPERATION_STATUS;

extern int SERVER_SOCKET;
extern int MAX_FD;
extern fd_set READ_FDS;
extern fd_set WRITE_FDS;
extern struct timeval TIMEOUT;

void CreateServerSocket();

void BindServerSocket();

void OpenPortForListening();

void MultiplexSocket();

void WaitForClients();

void InitServerSideNetwork();

#endif //SERVER_SERVER_NETWORK_H
