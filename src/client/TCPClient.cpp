#include <string>
#include <vector>
#include <mutex>
#include <common/socket.h>
#include <common/constants.h>
#include <iostream>
#include <thread>
#include <array>
#include <common/models/Message.h>
#include <iomanip>
#include <ctime>


class TCPClient {
public:

    TCPClient( const std::string& host, short port )
        : host_( host ), port_( port ) {
    }

    void start() {
        INIT_SOCKET();

        int client_fd = createTCPIpv4Socket();

        if (client_fd < 0) {
            std::cerr << "Error creating socket\n";
        }

        sockaddr_in server_address = createIpv4Address( host_, port_ );

        int result_connect = connect( client_fd, reinterpret_cast<sockaddr*>( &server_address ), sizeof( server_address ) );

        if (result_connect < 0) {
            std::cerr << "Error connecting to server\n";
            return;
        }


        std::array<char, 2048> buffer;
        std::string input;

        std::thread(
            [&]() {
                TCPClient::handleIncomingMessages( client_fd );
            }
        ).detach();

        std::srand( static_cast<unsigned>( std::time( nullptr ) ) );
        const int user_id = rand() % 100;

        while (true) {

            std::getline( std::cin, input );
            auto now = std::chrono::system_clock::now();
            std::time_t current_time = std::chrono::system_clock::to_time_t( now );

            Message msg( user_id, 0, current_time, input );

            msg.serialize( buffer );

            send( client_fd, buffer.data(), buffer.size(), 0 );
        }

        CLOSE_SOCKET( client_fd );
        CLEANUP_SOCKET();
    }

private:

    void handleIncomingMessages( int client_fd ) {

        std::array<char, 2048> buffer;

        while (true) {

            ssize_t bytes_received = recv( client_fd, buffer.data(), buffer.size(), 0 );

            if (bytes_received < 0) {
                std::cout << "Connection close by the server\n";
                break;
            }

            Message msg( Message::deserialize( buffer ) );

            std::tm* localtime = std::localtime( &msg.time );

            std::cout << std::put_time( localtime, "%d-%m-%y %H:%M:%S" ) << " " << msg.sender_id << ":" << msg.content << '\n';
        }

    }

    std::string host_;
    short port_;
};

int main() {

    TCPClient client( "127.0.0.1", PORT );
    client.start();

}