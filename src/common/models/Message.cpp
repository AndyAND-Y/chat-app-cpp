#include "Message.h"

// Constructor to initialize a Message object
Message::Message( int sender_id, int chat_id, std::time_t time, const std::string& message )
    : sender_id( sender_id ), chat_id( chat_id ), time( time ), message( message ) {
}

void Message::serialize( std::array<char, 2048>& buffer ) const /// const here means it cannot modify the struct 
{
    // Copy sender_id and chat_id
    int offset = 0;
    size_t message_length = message.size();

    std::memcpy( buffer.data() + offset, &sender_id, sizeof( sender_id ) );
    offset += sizeof( sender_id );

    std::memcpy( buffer.data() + offset, &chat_id, sizeof( chat_id ) );
    offset += sizeof( chat_id );

    std::memcpy( buffer.data() + offset, &time, sizeof( time ) );
    offset += sizeof( time );

    std::memcpy( buffer.data() + offset, &message_length, sizeof( message_length ) );
    offset += sizeof( message_length );

    std::memcpy( buffer.data() + offset, message.c_str(), message_length );
}

Message Message::deserialize( const std::array<char, 2048>& buffer )
{
    int sender_id;
    int chat_id;
    std::time_t time;
    size_t message_length;

    int offset = 0;

    std::memcpy( &sender_id, buffer.data() + offset, sizeof( sender_id ) );
    offset += sizeof( sender_id );

    std::memcpy( &chat_id, buffer.data() + offset, sizeof( chat_id ) );
    offset += sizeof( chat_id );

    std::memcpy( &time, buffer.data() + offset, sizeof( time ) );
    offset += sizeof( time );

    std::memcpy( &message_length, buffer.data() + offset, sizeof( message_length ) );
    offset += sizeof( message_length );

    std::string message( buffer.data() + offset, message_length );

    return Message( sender_id, chat_id, time, message );
}
