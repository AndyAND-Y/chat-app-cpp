#ifndef SOCKET_H
#define SOCKET_H

#pragma once

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define CLOSE_SOCKET(s) closesocket(s)
#define INIT_SOCKET() { \
        WSADATA wsaData; \
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { \
            std::cerr << "Winsock initialization failed\n"; \
            exit(1); \
        } \
    }
#define CLEANUP_SOCKET() WSACleanup()
#else
#include <unistd.h>  // For close()
#include <arpa/inet.h>
#include <netinet/in.h>
#define CLOSE_SOCKET(s) close(s)
#define INIT_SOCKET()
#define CLEANUP_SOCKET()
#endif

int createTCPIpv4Socket();
sockaddr_in createIpv4Address( std::string host, short port );

#endif
