#include "./TinyAES/aes.h"
#include "../config/Keys.h"
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>

namespace Encrpyt
{

    void padString(uint8_t* input, size_t& length) {
        size_t padding = 16 - (length % 16);
        for (size_t i = length; i < length + padding; ++i) {
            input[i] = padding;
        }
        length += padding;
    }

    // Unpadding function to remove padding from the decrypted data
    size_t unpadString(uint8_t* input, size_t length) {
        size_t padding = input[length - 1];
        return length - padding;
    }

    std::string bytesToHexString(const uint8_t* bytes, size_t length) 
    {
        std::ostringstream oss;
        for (size_t i = 0; i < length; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
        }
        return oss.str();
    }

    void hexStringToBytes(const std::string& hex, uint8_t* bytes) {
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        bytes[i / 2] = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
    }
}

    const std::string EncryptJWT(const std::string token)
    {
        size_t len = token.length();

        size_t paddedLength = len;
        uint8_t* paddedToken = new uint8_t[paddedLength + 16];
        memcpy(paddedToken, token.c_str(), len);
        padString(paddedToken, paddedLength);

        AES_ctx ctx;
        AES_init_ctx(&ctx, Chabi::AESKEY);

        for (size_t i = 0; i < paddedLength; i+=16)
        {
            AES_ECB_encrypt(&ctx, paddedToken + i);
        }

        std::string return_str = bytesToHexString(paddedToken, paddedLength);
        delete[] paddedToken;
        
        return return_str;
    }

    const std::string DecyptJWT(const std::string token)
    {
        size_t len = token.length() / 2;
        uint8_t* encryptedBytes = new uint8_t[len];

        hexStringToBytes(token, encryptedBytes);

        AES_ctx ctx;
        AES_init_ctx(&ctx, Chabi::AESKEY);

        for (size_t i = 0; i < len; i+=16)
        {
            AES_ECB_decrypt(&ctx, encryptedBytes + i);
        }

        size_t originalLength = unpadString(encryptedBytes, len);
        std::string decryptedText(reinterpret_cast<char*>(encryptedBytes), originalLength);
        delete[] encryptedBytes;

        return decryptedText;
    }

}