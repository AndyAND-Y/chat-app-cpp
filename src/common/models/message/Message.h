#ifndef MESSAGE_H
#define MESSAGE_H

#pragma once

#include <string>
#include <chrono>
#include <array>

enum class MessageType
{
    Register,
    Login,
    Exit,
    Chat,
};

class Message
{
private:
    MessageType message_type_;
    int sender_id_;
    std::time_t time_;
    std::string content_;

protected:

    Message( MessageType message_type, int sender_id, std::time_t time, const std::string& content )
        : message_type_( message_type ), sender_id_( sender_id ), time_( time ), content_( content ) {
    }

public:

    virtual ~Message() = default;

    virtual void serialize( std::array<char, 2048>& buffer ) const = 0;

    MessageType get_message_type() const { return message_type_; }
    int get_sender_id() const { return sender_id_; }
    std::time_t get_time() const { return time_; }
    const std::string& get_content() const { return content_; }
};

#endif