#include "../include/Auth.h"

std::unordered_map<std::string, std::string> parse_form_data(const std::string& body) {
    std::unordered_map<std::string, std::string> form_data;
    size_t start = 0;
    while (start < body.size()) {
        size_t end = body.find('&', start);
        if (end == std::string::npos) {
            end = body.size();
        }
        std::string pair = body.substr(start, end - start);
        size_t equal_pos = pair.find('=');
        if (equal_pos != std::string::npos) {
            std::string key = pair.substr(0, equal_pos);
            std::string value = pair.substr(equal_pos + 1);
            form_data[key] = value;
        }
        start = end + 1;
    }
    return form_data;
}

bool check_user_credentials (const std::string& username, const std::string password){

    sqlite3* db = open_database();

    if (!db) return false;

    std::string sql = "SELECT COUNT(*) FROM Users WHERE user_name = ? AND user_password = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Query me dikkat hai\n";
        std::cerr << "SQL Error: " << sqlite3_errmsg(db) << std::endl;
        close_database(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    std::cout << "Username: " << username << "\nPassword: " << password << "\nResult: " << result << "\n";
    bool valid = false;
    if (result == SQLITE_ROW) {
        valid = (sqlite3_column_int(stmt, 0) > 0);
    }

    sqlite3_finalize(stmt);
    close_database(db);
    return valid;
}

bool ADD_User (const std::string& username, const std::string password, const std::string email) {

    sqlite3* db = open_database();

    if (!db){ return false; }


    std::string sql_query_insertion = "INSERT INTO Users (user_name, user_password, user_email) VALUES (?, ?, ?)";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql_query_insertion.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Query me dikkat hai\n";
        close_database(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);

    auto result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    close_database(db);
    return result == SQLITE_DONE;
}