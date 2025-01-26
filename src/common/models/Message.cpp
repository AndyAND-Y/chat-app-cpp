#include "Message.h"
#include <iostream>

Message::Message( int sender_id, int chat_id, std::time_t time, const std::string& content )
    : sender_id( sender_id ), chat_id( chat_id ), time( time ), content( content ) {
}

void Message::serialize( std::array<char, 2048>& buffer ) const // const here means it cannot modify the struct 
{
    int offset = 0;
    size_t content_length = content.size();

    std::memcpy( buffer.data() + offset, &sender_id, sizeof( sender_id ) );
    offset += sizeof( sender_id );

    std::memcpy( buffer.data() + offset, &chat_id, sizeof( chat_id ) );
    offset += sizeof( chat_id );

    std::memcpy( buffer.data() + offset, &time, sizeof( time ) );
    offset += sizeof( time );

    std::memcpy( buffer.data() + offset, &content_length, sizeof( content_length ) );
    offset += sizeof( content_length );

    std::memcpy( buffer.data() + offset, content.c_str(), content_length );
}

Message Message::deserialize( const std::array<char, 2048>& buffer )
{
    int sender_id;
    int chat_id;
    std::time_t time;
    size_t content_length;

    int offset = 0;

    std::memcpy( &sender_id, buffer.data() + offset, sizeof( sender_id ) );
    offset += sizeof( sender_id );

    std::memcpy( &chat_id, buffer.data() + offset, sizeof( chat_id ) );
    offset += sizeof( chat_id );

    std::memcpy( &time, buffer.data() + offset, sizeof( time ) );
    offset += sizeof( time );

    std::memcpy( &content_length, buffer.data() + offset, sizeof( content_length ) );
    offset += sizeof( content_length );

    std::string content( buffer.data() + offset, content_length );

    return Message( sender_id, chat_id, time, content );
}
