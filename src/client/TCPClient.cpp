#include <string>
#include <vector>
#include <mutex>
#include <common/socket.h>
#include <common/constants.h>
#include <iostream>
#include <thread>
#include <array>
#include <common/models/message/Messages.h>
#include <iomanip>
#include <ctime>
#include <client/TCPClient.h>


TCPClient::TCPClient( const std::string& host, short port )
    : host_( host ), port_( port ), client_fd_( -1 ) {
}

void TCPClient::start() {

    connect_to_server();

    std::string input;

    std::thread(
        [&]() {
            TCPClient::handleIncomingMessages();
        }
    ).detach();

    std::srand( static_cast<unsigned>( std::time( nullptr ) ) );
    const int user_id = rand() % 100;

    while (true) {

        std::getline( std::cin, input );
        auto now = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t( now );

        ChatMessage msg( user_id, 0, current_time, input );

        send_message( msg );
    }

    disconnect_from_server();
}

void TCPClient::connect_to_server() {
    INIT_SOCKET();

    client_fd_ = createTCPIpv4Socket();

    if (client_fd_ < 0) {
        std::cerr << "Error creating socket\n";
    }

    sockaddr_in server_address = createIpv4Address( host_, port_ );

    int result_connect = connect( client_fd_, reinterpret_cast<sockaddr*>( &server_address ), sizeof( server_address ) );

    if (result_connect < 0) {
        std::cerr << "Error connecting to server\n";
        return;
    }
}

void TCPClient::disconnect_from_server() {
    CLOSE_SOCKET( client_fd_ );
    CLEANUP_SOCKET();
    client_fd_ = -1;
}

void TCPClient::send_message( const Message& msg ) {
    std::array<char, 2048> buffer;
    msg.serialize( buffer );
    send( client_fd_, buffer.data(), buffer.size(), 0 );
}

void TCPClient::handleIncomingMessages() { // TODO: implement taking a function argument to handle message

    std::array<char, 2048> buffer;

    while (true) {

        ssize_t bytes_received = recv( client_fd_, buffer.data(), buffer.size(), 0 );

        if (bytes_received < 0) {
            std::cout << "Connection close by the server\n";
            break;
        }

        ChatMessage msg( ChatMessage::deserialize( buffer ) );

        std::cout << msg.to_string();
    }
}

bool TCPClient::is_connected() {
    return client_fd_ > 0;
}