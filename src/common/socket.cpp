#include "socket.h"
#include <string>


int createTCPIpv4Socket() {
    return socket( AF_INET, SOCK_STREAM, 0 );
}

sockaddr_in createIpv4Address( std::string host, short port ) {

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( port );
    inet_pton( AF_INET, host.c_str(), &server_addr.sin_addr );

    return server_addr;
}