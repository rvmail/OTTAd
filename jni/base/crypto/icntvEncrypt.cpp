#include "icntvEncrypt.h"
#include "aes.h"
#include "base64.h"
#include "debug.h"
#include "base/utils/misc.h"
#include <cstdio>
#include <iostream>

#include <Poco/MD5Engine.h>
#include <Poco/DigestEngine.h>

#define KEYLEN 16

icntvEncrypt::icntvEncrypt(void)
{
}

icntvEncrypt::~icntvEncrypt(void)
{
}

std::string icntvEncrypt::md5Encrypt(const std::string src)
{
    std::string dst = "";

    Poco::MD5Engine md5;
    md5.update(src);
    Poco::DigestEngine::Digest digest = md5.digest();

    dst = Poco::DigestEngine::digestToHex(digest);

    LOGDEBUG("md5Encrypt: %s\n", dst.c_str());

    return dst;
}

std::string icntvEncrypt::aesEncrypt(const std::string src, const std::string key)
{
    std::string dst = "";

    unsigned int content_size =  src.length();
    unsigned int block_count = 0;
    unsigned int number_of_blocks = content_size/KEYLEN + ((content_size%KEYLEN)?1:0);
    uint8_t padding;
    uint8_t* data_block = (uint8_t*) malloc(KEYLEN);
    uint8_t* processed_block = (uint8_t*) malloc(KEYLEN);

    std::string src_line = "";

    for (block_count = 1; block_count <= number_of_blocks; block_count++)
    {
        memset(data_block, 0, KEYLEN);
        memset(processed_block, 0, KEYLEN);

        src_line = src.substr((block_count-1)*KEYLEN, KEYLEN);
        strncpy((char*)data_block, src_line.c_str(), src_line.length());

        if (block_count == number_of_blocks)
        {
            padding = KEYLEN - content_size%KEYLEN;
            if (padding < KEYLEN)
            {
                memset((data_block + KEYLEN - padding), (uint8_t)padding, padding);
            }

            AES128_ECB_encrypt(data_block, reinterpret_cast<const uint8_t*>(key.c_str()), processed_block);
            dst.append((char *)processed_block, KEYLEN);

            if (padding == KEYLEN)
            {
                memset(data_block, (uint8_t)padding, KEYLEN);
                AES128_ECB_encrypt(data_block, reinterpret_cast<const uint8_t*>(key.c_str()), processed_block);
                dst.append((char *)processed_block, KEYLEN);
            }
        }
        else
        {
            AES128_ECB_encrypt(data_block, reinterpret_cast<const uint8_t*>(key.c_str()), processed_block);
            dst.append((char *)processed_block, KEYLEN);
        }
    }

    free(data_block);
    free(processed_block);

    return hex_encode(dst);
    //return base64_encode(reinterpret_cast<const unsigned char*>(dst.c_str()), dst.length());
}

std::string icntvEncrypt::aesDecrypt(const std::string src, const std::string key)
{
    std::string dst = "";

    std::string encrypt_str = base64_decode(src);
    //std::string encrypt_str = hex_decode(src);

    unsigned int content_size = encrypt_str.length();
    if (content_size%KEYLEN != 0)
    {
        LOGERROR("Ciphertext Format Error!\n");
        return "";
    }

    unsigned int block_count = 0, number_of_blocks = content_size  / KEYLEN;
    uint8_t padding;
    uint8_t* data_block = (uint8_t*) malloc(KEYLEN);
    uint8_t* processed_block = (uint8_t*) malloc(KEYLEN);

    std::string encrypt_line = "";

    for (block_count = 1; block_count <= number_of_blocks; block_count++)
    {
        memset(data_block, 0, KEYLEN);
        memset(processed_block, 0, KEYLEN);

        encrypt_line = encrypt_str.substr((block_count-1)*KEYLEN, KEYLEN);
        memcpy(data_block, encrypt_line.c_str(), encrypt_line.length());

        if (block_count == number_of_blocks)
        {
            AES128_ECB_decrypt(data_block, reinterpret_cast<const uint8_t*>(key.c_str()), processed_block);
            padding = processed_block[KEYLEN-1];

            if (padding < KEYLEN)
            {
                dst.append((char*)processed_block, KEYLEN-padding);
            }
        }
        else
        {
            AES128_ECB_decrypt(data_block, reinterpret_cast<const uint8_t*>(key.c_str()), processed_block);
            dst.append((char*)processed_block, KEYLEN);
        }
    }

    free(data_block);
    free(processed_block);

    LOGDEBUG("icntvEncrypt::aesDecrypt: %s\n", dst.c_str());
    return dst;
}

std::string icntvEncrypt::hex_encode(const std::string &src)
{
    std::string dst = "";
    char tmp[5] = "";

    for(auto it=src.cbegin(); it != src.cend(); ++it)
    {
        memset(tmp, 0, 5);
        sprintf(tmp, "%.2X", *it);
        dst += tmp;
    }

    return dst;
}

std::string icntvEncrypt::hex_decode(const std::string &src)
{
    std::string dst = "";
    unsigned int tmp = 0;

    for (size_t i = 0; i < src.length(); i+=2)
    {
        sscanf(src.substr(i, 2).c_str(), "%02X", &tmp);
        dst += (char) tmp;
    }

    return dst;
}
