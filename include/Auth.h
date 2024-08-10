#pragma once

#include "database.h"

std::unordered_map<std::string, std::string> parse_form_data(const std::string& body);
bool check_user_credentials (const std::string& username, const std::string password);
bool ADD_User (const std::string& username, const std::string password, const std::string email);