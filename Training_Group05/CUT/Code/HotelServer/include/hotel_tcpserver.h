#ifndef HOTEL_TCPSERVER_H
#define HOTEL_TCPSERVER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netdb.h>
#include <vector>
#include <errno.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <bits/stdc++.h>
#include <common.h>

#define MAX_CLIENTS 4

using namespace std;

class TcpServer
{
private:
    int  server_fd;
    int client_fd[MAX_CLIENTS];
    pthread_t thread[MAX_CLIENTS];
    struct sockaddr_in server_addr;
    static map<int,Messaging> clientMessageMap;
    pthread_t _receiveThread;
    void init_server();
    void start_server();
public:
    TcpServer();
    ~TcpServer();
    static void* client_handler(void *client_fd);
    void wait_for_client_termination();
     
};

#endif

