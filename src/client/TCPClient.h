#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <array>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <common/socket.h>
#include <common/constants.h>
#include <common/models/message/Messages.h>

class TCPClient {
public:
    TCPClient( const std::string& host, short port );
    void start();
    void connect_to_server();
    void disconnect_from_server();
    void send_message( const Message& msg );
    void handleIncomingMessages();
    bool is_connected();

private:
    int client_fd_;
    std::string host_;
    short port_;
};

#endif