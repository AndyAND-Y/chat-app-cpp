#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <common/constants.h>
#include <common/socket.h>

void receiveMessages(int sockfd)
{
    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            std::cerr << "Connection lost." << std::endl;
            break;
        }
        std::cout << "\n[C] Message Received: " << buffer << std::endl;
    }
}

int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSA STARTUP FAILED!" << std::endl;
        return -1;
    }
#endif

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Socket creation failed!" << std::endl;
        cleanup();
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Connection failed!" << std::endl;
        closeSocket(sockfd);
        cleanup();
        return -1;
    }

    std::cout << "Connected to server on port " << PORT << std::endl;

    std::thread(receiveMessages, sockfd).detach();

    std::string message;
    while (true)
    {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        if (message == "/exit")
        {
            break;
        }

        send(sockfd, message.c_str(), message.size(), 0);
    }

    closeSocket(sockfd);
    cleanup();
    return 0;
}
