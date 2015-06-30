#include "icntvEncrypt.h"
#include "base/utils/log.h"
#include "base/utils/string.h"

#include <Poco/MD5Engine.h>
#include <Poco/DigestEngine.h>

icntvEncrypt::icntvEncrypt(void) : encryption(algorithm_md5)
{
}

icntvEncrypt::icntvEncrypt(const std::string crypt)
{
	encryption = crypt;
}

icntvEncrypt::~icntvEncrypt(void)
{
}

void icntvEncrypt::setToken( const char* token )
{

}

std::string icntvEncrypt::encrypt( const std::string src )
{
	if (compareCaseInsensitive(encryption, algorithm_md5))
	{
		LOG(DEBUG) << "algorithm_md5";
		return md5Encrypt(src);
	}
	else if (compareCaseInsensitive(encryption, algorithm_sha1))
	{
		LOG(DEBUG) << "algorithm_sha1";
		return "";
	}
	else
	{
		LOG(DEBUG) << "no match encryption algorithm,  use algorithm_md5";
		return md5Encrypt(src);
	}
}

int icntvEncrypt::decrypt( const char* src, char* dst )
{
	int nRet = 0;
	return nRet;
}

std::string icntvEncrypt::md5Encrypt(const std::string src)
{
	std::string dst = "";

    Poco::MD5Engine md5;
    md5.update(src);
    Poco::DigestEngine::Digest digest = md5.digest();

    dst = Poco::DigestEngine::digestToHex(digest);

    LOG(DEBUG) << "md5Encrypt : " << dst;

    return dst;
}
