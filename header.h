#pragma once

#include <crow.h>
#include <sqlite3.h>
#include <sstream>
#include <string>
#include <unordered_map>


struct Chat {
    int id;
    std::string name;

    crow::json::wvalue to_json() const;
};


struct Message
{
    int m_id;
    int s_id;
    std::string message;
    std::string time;
};

struct ChatInfo {
    int id;                    // Unique identifier for the chat
    std::string name;          // Name of the chat
    std::string last_message_text; // Text of the last message
    std::string last_message_time; // Timestamp of the last message
    bool is_group;             // Whether the chat is a group chat
};