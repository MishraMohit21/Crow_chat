#pragma once

#include "../header.h"

sqlite3* open_database();
void close_database(sqlite3* db);
std::vector<Chat> get_user_chats(const std::string& username);
std::vector<Message> get_chat_messages(int chat_id);
ChatInfo get_chat_info(int chat_id);
