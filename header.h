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