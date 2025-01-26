#ifndef MESSAGE_H
#define MESSAGE_H

#pragma once

#include <cstring>
#include <chrono>
#include <array>


struct Message
{
    int sender_id;
    int chat_id;
    std::time_t time;
    std::string content;

    Message( int sender_id, int chat_id, std::time_t time, const std::string& content );

    void serialize( std::array<char, 2048>& buffer ) const;
    static Message deserialize( const std::array<char, 2048>& buffer );

};

#endif