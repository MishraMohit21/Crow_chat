#include "include/Auth.h"
#include "include/jwt.h"
#include "include/Encrypt.h"



void crow_routes_ka_function(crow::SimpleApp &app) {


    CROW_ROUTE (app, "/") ([](){
        auto page = crow::mustache::load_text("Home.html");
        return page;
    });

    CROW_ROUTE (app, "/login")([](){
        auto page = crow::mustache::load_text("user_login.html");
        return page;
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST) ([](const crow::request& req){
        auto from_data = parse_form_data(req.body);
        auto username_it = from_data.find("username");
        auto password_it = from_data.find("password");

        if (username_it != from_data.end() && password_it != from_data.end()) {
            auto username = username_it->second;
            auto password = password_it->second;

            if (check_user_credentials(username, password)) {
                std::string token = JWT::GenerateJWT(username);

                crow::response res(302);

                std::string encToken = Encrpyt::EncryptJWT(token);

                // res.add_header("Set-Cookie", "jwt=" + token);
                res.add_header("Set-Cookie", "jwt=" + encToken + "; HttpOnly; Secure; SameSite=Strict; Path=/");  //Isse tum cookie ke storage se toh kar sakte ho but js console se nahi kar sakte ho

                res.add_header("Location", "/dashboard");
                return res;
            }
        }
        crow::response res(302);
        res.add_header("Location", "/login");
        return res;
    });

    // Verify JWT on /dashboard
    CROW_ROUTE(app, "/dashboard")([](const crow::request& req) {
        std::string jwt_token = req.get_header_value("Cookie");
        std::string extra = "jwt=";
        jwt_token.erase(0, extra.length());

        std::string decToken = Encrpyt::DecyptJWT(jwt_token);

        if (JWT::Verify_jwt(decToken)) {
            auto page = crow::mustache::load("dashboard.html");
            crow::mustache::context cntx;

            std::string username = jwt::decode(decToken).get_payload_claim("username").as_string();
            cntx["username"] = username;

            auto chats = get_user_chats(username);
            std::vector<crow::json::wvalue> json_chats;

            for (auto &chat : chats) {
                crow::json::wvalue chat_data;
                chat_data["chat_id"] = chat.id;
                chat_data["chat_name"] = chat.name;

                json_chats.push_back(chat_data);
            }
            cntx["chats"] = std::move(json_chats);
            
            // Check if a specific chat_id is requested
            auto chat_id = req.url_params.get("chat_id");
        
            CROW_LOG_INFO << "Chat id of the clicked chat is " << chat_id;
            if (chat_id) {
                int chat_id_int = std::stoi(chat_id);
                auto chat_messages = get_chat_messages(chat_id_int);
                // auto chat_info = get_chat_info(chat_id_int);
                // std::cout << "Size of Chats: " << chat_messages.size() << std::endl;
                std::vector<crow::json::wvalue> messages_json;
                int i = 0;
                for (auto &message : chat_messages) 
                {

                    crow::json::wvalue message_data;
                    message_data["message_id"] = message.m_id;
                    message_data["sender_id"] = message.s_id;
                    message_data["message_text"] = message.message;
                    message_data["message_time"] = message.time;
                    
                    messages_json.push_back(message_data);
                }


                // cntx["current_chat_name"] = chat_info.name;
                cntx["messages"] = std::move(messages_json);
            } else {
                // cntx["chat_name"] = "Select a chat";
                cntx["messages"] = crow::json::wvalue{};
            }

            CROW_LOG_INFO << "Contex ka dump: " << cntx.dump();

            return crow::response{page.render(cntx)};
        }
        return crow::response{403, "Forbidden"};
    });


    CROW_ROUTE (app, "/signup")([](){

        auto page = crow::mustache::load_text("user_signup.html");
        return page;
    });

    CROW_ROUTE (app, "/signup").methods(crow::HTTPMethod::POST)([](const crow::request &req){

        auto data_from_req = parse_form_data(req.body);

        auto username_it = data_from_req.find("username");
        auto password_it = data_from_req.find("password");
        auto email_it = data_from_req.find("email");

        if (username_it != data_from_req.end() && password_it != data_from_req.end() && email_it != data_from_req.end()) 
        {

            auto username = username_it->second;
            auto password = password_it->second;
            auto email = email_it->second;

            ADD_User(username, password, email);

            crow::response res(302);
            res.add_header("Location", "/login");
            return res;
        }
        else {
            crow::response res(400);
            res.add_header("Location", "/");
            return res;
        }
    });


    CROW_ROUTE (app, "/test") ([](){
        auto page = crow::mustache::load_text("test.html");
        return page;
    });


    // CROW_ROUTE (app, "/dashboard") ([](const crow::request& req){

    //     auto page = crow::mustache::load("dashboard.html");
    //     crow::mustache::context cntx;

    //     std::string username = req.url_params.get("username");
    //     cntx["username"] = username;

    //     std::vector<crow::json::wvalue> json_chats;

    //     auto chats = get_user_chats(username);
    //     if (!chats.empty())
    //     {
    //         for (auto &chat : chats)
    //         {   
    //             // std::cout << "Chat detail:\n Name: " << chat.name << "\n";
    //             json_chats.push_back(chat.to_json());
    //         }
    //         std::cout << json_chats[0].size() << "\n";
    //         cntx["chats"] = std::move(json_chats);

    //     }
    //     cntx["chat_name"] = chats[0].name;
    //     CROW_LOG_INFO << "Context data: " << cntx.dump();
    //     return crow::response{page.render(cntx)};
    // });


}


int main() {

    crow::SimpleApp app;

    crow_routes_ka_function(app);

    app.port(18080).multithreaded().run();
}