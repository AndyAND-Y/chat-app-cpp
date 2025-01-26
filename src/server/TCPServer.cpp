#include <string>
#include <vector>
#include <mutex>
#include <common/socket.h>
#include <common/constants.h>
#include <iostream>
#include <thread>
#include <array>
#include <common/models/Message.h>
#include <ctime>

class TCPServer {
public:

    TCPServer( const std::string& host, short port )
        : host_( host ), port_( port ) {
    }

    void start() {
        std::cout << "Server starting...\n";
        INIT_SOCKET();

        int server_fd = createTCPIpv4Socket();

        if (server_fd < 0) {
            std::cerr << "Error creating socket\n";
        }

        sockaddr_in server_address = createIpv4Address( host_, port_ );

        int result_bind = bind( server_fd, reinterpret_cast<sockaddr*>( &server_address ), sizeof( server_address ) );

        if (result_bind < 0) {
            std::cerr << "Error binding the socket\n";
            return;
        }


        int result_listen = listen( server_fd, 5 );

        if (result_listen < 0) {
            std::cerr << "Error listening on socket\n";
            return;
        }

        std::cout << "Server listening on " << host_ << ":" << port_ << '\n';

        while (true) {
            sockaddr_in client_address{};
            socklen_t client_len = sizeof( client_address );

            int client_fd = accept( server_fd, reinterpret_cast<sockaddr*>( &client_address ), &client_len );

            if (client_fd < 0) {
                std::cerr << "Error accepting connection\n";
            }

            {
                std::lock_guard<std::mutex> lock( clients_mutex_ );
                clients_.push_back( client_fd );
                std::cout << "Clients: ";
                for (auto client : clients_) {
                    std::cout << client << ", ";
                }

                std::cout << '\n';
            }

            std::thread(
                [&]() {
                    TCPServer::handle_client( client_fd );
                }
            ).detach();

        }


        CLOSE_SOCKET( server_fd );
        CLEANUP_SOCKET();
    }


private:

    void handle_client( int client_fd ) {

        std::array<char, 2048> buffer;

        while (true) {
            buffer.fill( 0 );

            ssize_t bytes_received = recv( client_fd, buffer.data(), buffer.size(), 0 );

            if (bytes_received <= 0) {
                std::cout << "Connection closed by the user\n";
                break;
            }

            Message msg( Message::deserialize( buffer ) );

            {
                std::lock_guard<std::mutex> lock( clients_mutex_ );

                for (int client : clients_) {
                    send( client, buffer.data(), buffer.size(), 0 );
                    std::cout << client_fd << " -> " << client << '\n';
                }

            }

        }

        {
            std::lock_guard<std::mutex> lock( clients_mutex_ );
            clients_.erase(
                std::remove_if(
                    clients_.begin(),
                    clients_.end(),
                    [client_fd]( int client_id )
                    {
                        return client_id == client_fd;
                    } ) );

            CLOSE_SOCKET( client_fd );

            std::cout << "Clients: ";
            for (auto client : clients_) {
                std::cout << client << ", ";
            }

            std::cout << '\n';

            auto now = std::chrono::system_clock::now();
            std::time_t current_time = std::chrono::system_clock::to_time_t( now );

            std::string content = "User disconected!";

            Message serverMessage( 0, 0, current_time, content );

            serverMessage.serialize( buffer );

            for (int client : clients_) {
                send( client, buffer.data(), buffer.size(), 0 );
                std::cout << client_fd << " -> " << client << '\n';
            }

        }

    }

    std::string host_;
    short port_;
    std::vector<int> clients_;
    std::mutex clients_mutex_;

};

int main() {

    TCPServer server( "127.0.0.1", PORT );
    server.start();

}