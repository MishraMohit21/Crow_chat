#include "database.h"


const std::string DB_PATH = "./ChatApp.db";

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