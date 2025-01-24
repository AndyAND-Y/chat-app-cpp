#include <server/handleClient.h>

void handleClientConnection(
    int clientFd,
    std::vector<int>& clients,
    std::mutex& clientsMutex,
    void ( *updateChat )( const Message& message )
) {
    char buffer[2048];

    while (true)
    {
        memset( buffer, 0, sizeof( buffer ) );
        int bytesReceived = recv( clientFd, buffer, sizeof( buffer ), 0 );

        if (bytesReceived <= 0)
        {
            std::lock_guard<std::mutex> lock( clientsMutex );
            clients.erase(
                std::remove_if(
                    clients.begin(),
                    clients.end(),
                    [clientFd]( int clientId )
                    {
                        return clientId == clientFd;
                    } ) );
        }

        std::time_t now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );

        std::string messageContent( buffer );

        const Message message( clientFd, 0, now, messageContent );

        updateChat( message );
    }
}