#include "include/Auth.h"


void crow_routes_ka_function(crow::SimpleApp &app) {


    CROW_ROUTE (app, "/") ([](){
        auto page = crow::mustache::load_text("Home.html");
        return page;
    });

    CROW_ROUTE (app, "/login")([](){
        auto page = crow::mustache::load_text("user_login.html");
        return page;
    });

    CROW_ROUTE (app, "/login").methods(crow::HTTPMethod::POST) ([](const crow::request& req){
        // auto page = crow::mustache::load_text("user_login.html");
        // return page;

        auto from_data = parse_form_data(req.body);

        auto username_it = from_data.find("username");
        auto password_it = from_data.find("password");

        if (username_it != from_data.end() && password_it != from_data.end()) {

            auto username = username_it->second;
            auto password = password_it->second;

            if (check_user_credentials(username, password)) {

                crow::response res(302);
                res.add_header("Location", "/dashboard?username=" + username);
                return res;
                // return crow::response::redirect("/")
            }
            else{
                crow::response res(302);
                res.add_header("Location", "/login?error=Incorrect%20password");
                return res;
                // return crow::response{302, "Found", {{"Location", "/login?error=Incorrect%20password"}}};
            }
        }
        else{
            crow::response res(304);
            res.add_header("Location", "/login");
            return res;
        }
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


    CROW_ROUTE (app, "/dashboard") ([](const crow::request& req){

        auto page = crow::mustache::load("dashboard.html");
        crow::mustache::context cntx;

        std::string username = req.url_params.get("username");
        cntx["username"] = username;

        std::vector<crow::json::wvalue> json_chats;

        auto chats = get_user_chats(username);
        if (!chats.empty())
        {
            for (auto &chat : chats)
            {   
                // std::cout << "Chat detail:\n Name: " << chat.name << "\n";
                json_chats.push_back(chat.to_json());
            }
            std::cout << json_chats[0].size() << "\n";
            cntx["chats"] = std::move(json_chats);

        }
        cntx["chat_name"] = chats[0].name;
        CROW_LOG_INFO << "Context data: " << cntx.dump();
        return crow::response{page.render(cntx)};
    });


}


int main() {

    crow::SimpleApp app;

    crow_routes_ka_function(app);

    app.port(18080).multithreaded().run();
}