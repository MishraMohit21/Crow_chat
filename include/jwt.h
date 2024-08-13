#pragma once

#include "jwt-cpp/jwt.h"
#include "../config/Keys.h"
#include <crow.h>

namespace JWT
{
    const std::string SECRET_KEY = Chabi::JWT_Key;
    
    std::string GenerateJWT(const std::string& username)
    {
        std::string token = jwt::create()
                                .set_issuer("auth0")
                                .set_type("JWS")
                                .set_payload_claim("username", jwt::claim(username))
                                .sign(jwt::algorithm::hs256{SECRET_KEY});
        // std::cout << "JWT Token given is: " << token << std::endl;
        return token;
    }

    bool Verify_jwt(const std::string& jwt_string)
    {
        try
        {
            auto decode = jwt::decode(jwt_string);
            auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{SECRET_KEY}).with_issuer("auth0");
            verifier.verify(decode);
            return true;
        }
        catch(const std::exception& e)
        {
            CROW_LOG_ERROR << "JWT verification failed: " << e.what();
            return false;
        }
        
    }
}