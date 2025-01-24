#include "socket.h"
#include <iostream>

#ifdef _WIN32
void cleanup()
{
    WSACleanup();
}
#else
void cleanup() {}
#endif

void closeSocket( int socket_fd )
{
#ifdef _WIN32
    closesocket( socket_fd );
#else
    close( socket_fd );
#endif
    cleanup();
}
