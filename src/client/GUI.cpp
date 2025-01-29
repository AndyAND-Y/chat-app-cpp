#include <iostream>
#include <string>
#include <client/TCPClient.h>
#include <common/models/Client.h>
#include <thread>

class GUI {

public:

    GUI() :
        connection_( "127.0.0.1", PORT )
    {
    }

    void start() {

        std::string input;

        connection_.connect_to_server();

        std::thread( &TCPClient::handleIncomingMessages, &connection_ ).detach();

        while (true) {
            std::getline( std::cin, input );

            std::cout << "\033[A\33[2K\r";

            auto now = std::chrono::system_clock::now();
            std::time_t current_time = std::chrono::system_clock::to_time_t( now );

            ChatMessage msg( 20, 1, current_time, input );



            connection_.send_message( msg );
        }

        connection_.disconnect_from_server();

    }

private:

    Client client_;
    TCPClient connection_;

};

int main() {

    GUI gui;
    gui.start();

};