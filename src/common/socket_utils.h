#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

void cleanup();
void closeSocket(int socket_fd);

#endif
