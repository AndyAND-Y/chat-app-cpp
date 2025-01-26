#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include "Message.h"
#include <cstring> 

class ChatMessage : public Message
{
private:
    int chat_id_;

public:
    ChatMessage( int sender_id, int chat_id, std::time_t time, const std::string& content )
        : Message( MessageType::Chat, sender_id, time, content ) {
    }

    std::string to_string() {

        std::ostringstream oss;

        char time_buffer[100];

        time_t time = get_time();
        std::tm time_lt = *std::localtime( &time );
        std::strftime( time_buffer, sizeof( time_buffer ), "%d-%m-%y %H:%M:%S", &time_lt );

        oss << time_buffer
            << " "
            << get_sender_id()
            << " "
            << get_content()
            << "\n";

        return oss.str();

    }


    void serialize( std::array<char, 2048>& buffer ) const override
    {
        buffer.fill( 0 );
        size_t offset = 0;

        int type = static_cast<int>( MessageType::Chat );
        std::memcpy( buffer.data() + offset, &type, sizeof( type ) );
        offset += sizeof( type );

        int sender_id = get_sender_id();

        std::memcpy( buffer.data() + offset, &sender_id, sizeof( sender_id ) );
        offset += sizeof( sender_id );

        time_t time = get_time();

        std::memcpy( buffer.data() + offset, &time, sizeof( time ) );
        offset += sizeof( time );

        std::string content = get_content();

        int content_length = content.size();

        std::memcpy( buffer.data() + offset, &content_length, sizeof( content_length ) );
        offset += sizeof( content_length );

        std::memcpy( buffer.data() + offset, content.data(), content.size() );

    }


    static ChatMessage deserialize( const std::array<char, 2048>& buffer )
    {
        size_t offset = 0;

        int type = 0;
        std::memcpy( &type, buffer.data() + offset, sizeof( type ) );
        offset += sizeof( type );

        int sender_id = 0;
        std::memcpy( &sender_id, buffer.data() + offset, sizeof( sender_id ) );
        offset += sizeof( sender_id );

        int chat_id = 0;
        std::memcpy( &chat_id, buffer.data() + offset, sizeof( chat_id ) );
        offset += sizeof( chat_id );

        std::time_t time = 0;
        std::memcpy( &time, buffer.data() + offset, sizeof( time ) );
        offset += sizeof( time );


        int content_length = 0;
        std::memcpy( &content_length, buffer.data() + offset, sizeof( content_length ) );
        offset += sizeof( content_length );


        std::string content;
        std::memcpy( content.data(), buffer.data() + offset, content_length );

        return ChatMessage( sender_id, chat_id, time, content );
    }

};

#endif
