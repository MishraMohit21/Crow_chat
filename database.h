#include "header.h"

sqlite3* open_database();
void close_database(sqlite3* db);
std::vector<Chat> get_user_chats(const std::string& username);