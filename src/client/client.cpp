#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <common/constants.h>
#include <common/socket.h>
#include <common/models/Message.h>
#include <array>

void receiveMessages( int sockfd ) {
    std::array<char, 2048> buffer;  // Using std::array to store incoming data

    while (true) {

        int bytesReceived = recv( sockfd, buffer.data(), buffer.size(), 0 );
        if (bytesReceived <= 0) {
            std::cerr << "Connection lost." << std::endl;
            break;
        }

        Message receivedMessage = Message::deserialize( buffer );

        std::cout << "\n[C] Message Received from client "
            << receivedMessage.sender_id << ": "
            << receivedMessage.message << std::endl;
    }
}


void sendMessage( int sockFd ) {
    std::string message;

    while (true) {
        std::cout << "Enter message: ";
        std::getline( std::cin, message );

        if (message == "/exit") {

            std::string exitMessage = "Client disconnected.";
            send( sockFd, exitMessage.c_str(), exitMessage.size(), 0 );
            break;
        }


        std::time_t now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        Message messageToSend( sockFd, 0, now, message );

        std::array<char, 2048> buffer;
        messageToSend.serialize( buffer );

        send( sockFd, buffer.data(), buffer.size(), 0 );
    }
}


int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0)
    {
        std::cerr << "WSA STARTUP FAILED!" << std::endl;
        return -1;
    }
#endif

    int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if (sockfd < 0)
    {
        std::cerr << "Socket creation failed!" << std::endl;
        cleanup();
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( PORT );
    server_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );

    if (connect( sockfd, (struct sockaddr*)&server_addr, sizeof( server_addr ) ) < 0)
    {
        std::cerr << "Connection failed!" << std::endl;
        closeSocket( sockfd );
        cleanup();
        return -1;
    }

    std::cout << "Connected to server on port " << PORT << std::endl;

    std::thread( receiveMessages, sockfd ).detach();

    std::string message;
    while (true)
    {
        std::cout << "Enter message: ";
        std::getline( std::cin, message );

        if (message == "/exit")
        {
            break;
        }

        send( sockfd, message.c_str(), message.size(), 0 );
    }

    closeSocket( sockfd );
    cleanup();
    return 0;
}
