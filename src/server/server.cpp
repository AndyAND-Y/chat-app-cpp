#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#include <common/socket_utils.h>
#include <common/constants.h>

std::vector<int> clients;
std::mutex clients_mutex;

void broadcastMessage(const std::string &message, int sender_fd)
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client : clients)
    {
        if (client != sender_fd)
        {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}

void handleClient(int client_fd)
{
    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            std::cerr << "Client disconnected: " << client_fd << std::endl;
            closeSocket(client_fd);
            {
                std::lock_guard<std::mutex> lock(clients_mutex);

                clients.erase(
                    std::remove_if(
                        clients.begin(),
                        clients.end(),
                        [client_fd](int client_id)
                        {
                            return client_id == client_fd;
                        }));
            }
            break;
        }
        std::string message = "Client " + std::to_string(client_fd) + ": " + buffer + '\n';
        std::cout << message;
        broadcastMessage(message, client_fd);
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
    int server_fd;
    sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd <= 0)
    {
        std::cerr << "SOCKET CREATION FAILED!" << std::endl;
        cleanup();
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "BIND FAILED" << std::endl;
        closeSocket(server_fd);
        return -1;
    }

    if (listen(server_fd, 5) < 0)
    {
        std::cerr << "LISTEN FAILED!" << std::endl;
        closeSocket(server_fd);
        return -1;
    }

    std::cout << "Server is running on port " << PORT << std::endl;

    while (true)
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0)
        {
            std::cerr << "Accept failed" << std::endl;
            closeSocket(server_fd);
            exit(EXIT_FAILURE);
        }

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_fd);
        }

        std::cout << "New client connected: " << client_fd << std::endl;
        std::thread(handleClient, client_fd).detach();
    }

    closeSocket(server_fd);
    return 0;
}
