#ifndef HOTEL_TCPCLIENT_H
#define HOTEL_TCPCLIENT_H

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

using namespace std;

class TcpClient
{
private:
    static int  _sockfd;
    static std::atomic<bool> _isConnected;
    static std::atomic<bool> _isClosed;
    static struct sockaddr_in _server;
    static pthread_t _receiveThread;
    static void initializeSocket();
    static void setAddress(const std::string& address, int port);
    static void terminateReceiveThread();

public:
    static Messaging _msg;
    static bool connectTo(const std::string & address, int port);
    static int sendMsg(const char * msg, size_t size);
    static bool isConnected() {  return _isConnected; }
    static void setIsConnected(bool opt) { _isConnected = opt; }
    static void waitForReceiveThread(); 
    static int close();
    static int getSockFd() { return _sockfd; }
};

#endif

