#include <common/constants.h>
#include <common/socket.h>
#include <server/handleClient.h>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::vector<int> clients;
std::mutex clients_mutex;

void updateChat( const Message& message ) {

    std::lock_guard<std::mutex> lock( clients_mutex );

    std::array<char, 2048> buffer;
    message.serialize( buffer );

    for (int clientFd : clients) {
        int bytesSent = send( clientFd, buffer.data(), buffer.size(), 0 );
        if (bytesSent <= 0) {
            std::cerr << "Failed to send message " << clientFd << '\n';
        }
    }

    std::cout << "Chat Update: " << message.sender_id << ' ' << message.message << '\n';

};

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0) {
        std::cerr << "WSA STARTUP FAILED!" << std::endl;
        return -1;
    }
#endif
    int server_fd;
    sockaddr_in server_addr;

    server_fd = socket( AF_INET, SOCK_STREAM, 0 );

    if (server_fd <= 0) {
        std::cerr << "SOCKET CREATION FAILED!" << std::endl;
        cleanup();
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons( PORT );

    if (bind( server_fd, (struct sockaddr*)&server_addr, sizeof( server_addr ) ) < 0) {
        std::cerr << "BIND FAILED" << std::endl;
        closeSocket( server_fd );
        return -1;
    }

    if (listen( server_fd, 5 ) < 0) {
        std::cerr << "LISTEN FAILED!" << std::endl;
        closeSocket( server_fd );
        return -1;
    }

    std::cout << "Server is running on port " << PORT << std::endl;

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof( client_addr );
        int client_fd =
            accept( server_fd, (struct sockaddr*)&client_addr, &client_len );
        if (client_fd < 0) {
            std::cerr << "Accept failed" << std::endl;
            closeSocket( server_fd );
            exit( EXIT_FAILURE );
        }

        {
            std::lock_guard<std::mutex> lock( clients_mutex );
            clients.push_back( client_fd );
        }

        std::cout << "New client connected: " << client_fd << std::endl;
        std::thread(
            [&]() {
                handleClientConnection( client_fd, clients, clients_mutex, updateChat );
            }
        ).detach();
    }

    closeSocket( server_fd );
    return 0;
}
