#include "../include/database.h"



const std::string DB_PATH = "./db/ChatApp.db";

sqlite3* open_database() {

    sqlite3* db;
    if (sqlite3_open(DB_PATH.c_str(), &db)){
        std::cout << "Not found the data\n";
        return nullptr;
    }
    return db;
}

void close_database(sqlite3* db) {
    sqlite3_close(db);
}


std::vector<Chat> get_user_chats(const std::string& username) 
{

    std::vector <Chat> chats;

    sqlite3* db = open_database();

    if (!db) return chats;

    std::string user_sql = "SELECT user_id FROM Users WHERE user_name = ?";
    sqlite3_stmt* user_stmt;
    int user_id = -1;

    if (sqlite3_prepare_v2(db, user_sql.c_str(), -1, &user_stmt, nullptr) != SQLITE_OK){

        std::cout << "User query error: " << sqlite3_errmsg(db) << "\n";
        close_database(db);
        return chats;
    }

    sqlite3_bind_text(user_stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(user_stmt) == SQLITE_ROW) 
        user_id = sqlite3_column_int(user_stmt, 0);
    
    
    sqlite3_finalize(user_stmt);

    if (user_id == -1) {
        close_database(db);
        return chats;
    }

    std::string sql = R"(
        SELECT c.chat_id, c.chat_name
        FROM Chats c
        JOIN UserChats uc ON c.chat_id = uc.chat_id
        WHERE uc.user_id = ?
    )";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr)) {
        std::cout << "Chat query error: " << sqlite3_errmsg(db) << "\n";
        close_database(db);
        return chats;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Chat chat;
        chat.id = sqlite3_column_int(stmt, 0);
        chat.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        chats.push_back(chat);
    }

    sqlite3_finalize(stmt);
    close_database(db);

    return chats;
}

crow::json::wvalue Chat::to_json() const {
        crow::json::wvalue json;
        json["id"] = id;
        json["name"] = name;
        return json;
    }


std::vector<Message> get_chat_messages(int chat_id) {
    std::vector<Message> messages;
    sqlite3* db = open_database();
    if (!db) return messages;

    std::string sql = R"(
        SELECT m.message_id, m.sender_id, m.message_text, m.message_time
        FROM Messages m
        WHERE m.chat_id = ?
        ORDER BY m.message_time ASC
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        close_database(db);
        return messages;
    }

    sqlite3_bind_int(stmt, 1, chat_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Message msg;
        msg.m_id = sqlite3_column_int(stmt, 0);              // Fetch the message ID
        msg.s_id = sqlite3_column_int(stmt, 1);       // Fetch the sender ID
        msg.message = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        msg.time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        messages.push_back(msg);
    }

    sqlite3_finalize(stmt);
    close_database(db);
    return messages;
}



ChatInfo get_chat_info(int chat_id) {
    ChatInfo chat_info;
    sqlite3* db = open_database();
    if (!db) return chat_info;

    std::string sql = R"(
        SELECT c.chat_id, c.chat_name, c.last_message_text, c.last_message_timestamp, c.is_group
        FROM Chats c
        WHERE c.chat_id = ?
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        close_database(db);
        return chat_info;
    }

    sqlite3_bind_int(stmt, 1, chat_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        chat_info.id = sqlite3_column_int(stmt, 0); // Fetch the chat ID
        chat_info.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)); // Fetch the chat name
        chat_info.last_message_text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)); // Fetch last message text
        chat_info.last_message_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)); // Fetch last message time
        chat_info.is_group = sqlite3_column_int(stmt, 4) != 0; // Fetch whether the chat is a group
    }

    // CROW_LOG_DEBUG << 

    sqlite3_finalize(stmt);
    close_database(db);
    return chat_info;
}   