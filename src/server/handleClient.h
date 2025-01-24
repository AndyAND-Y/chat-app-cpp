#ifndef HANDLECLIENT_H
#define HANDLECLIENT_H

#pragma once
#include <cstring>
#include <socket.h>
#include <mutex>
#include <algorithm>
#include <vector>
#include <string>
#include <common/models/Message.h>

void handleClientConnection(
    int clientFd,
    std::vector<int>& clients,
    std::mutex& clientsMutex,
    void ( *updateChat )( const Message& message )
);

#endif