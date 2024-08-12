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