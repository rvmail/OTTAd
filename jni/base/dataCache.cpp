#include "dataCache.h"
#include "base/utils/log.h"

dataCache* dataCache::m_pInstance = NULL;

dataCache::dataCache(void) : mLicense("")
{
    mPath.clear();
}

dataCache::~dataCache(void)
{
}

dataCache* dataCache::getInstance()
{
    if (m_pInstance  == NULL)
    {
        m_pInstance = new dataCache();
    }

    return m_pInstance;
}

void dataCache::setPath(string path)
{
    mPath = path;
}

string dataCache::getPath()
{
    return mPath;
}

int dataCache::getPath(char *path)
{
    int nLength = mPath.length();
    if (nLength <= 0)
    {
        return -1;
    }

    memcpy(path, mPath.c_str(), nLength);

    return 0;
}

void dataCache::setLicense(string license)
{
    mLicense = license;
}

string dataCache::getLicense()
{
    return mLicense;
}

