/**
 *
 * Copyright 2014 Future TV, Inc.
 *
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the License). You may not use this file except in
 * compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.icntv.tv/licenses/LICENSE-1.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: zml
 */

#include <stdio.h>
#include <unistd.h>
#include "basefile.h"
#include "log.h"

BaseFile::BaseFile()
{
    m_pFile = NULL;
}

BaseFile::~BaseFile()
{
    fileClose();
}

bool BaseFile::isExist(const char *filename)
{
    if (filename == NULL)
    {
        LOG(ERROR) << filename << " is NULL";
        return false;
    }

    if (access(filename, F_OK) != 0)
    {
        LOG(DEBUG) << filename << " is NOT exist!";
        return false;
    }

    LOG(DEBUG) << filename << " is exist!";
    return true;
}

bool BaseFile::fileOpen(const char *filename, const char *mode)
{
    if (filename == NULL)
    {
        LOG(ERROR) << filename << " is NULL";
        return false;
    }

    if (mode == NULL)
    {
       LOG(ERROR) << mode << " is NULL";
       return false;
    }

    m_pFile = fopen(filename, mode);
    if (m_pFile == NULL)
    {
        LOG(ERROR) << "fopen " << filename << " error";
        return false;
    }

    return true;
}

int BaseFile::getSize()
{
    if (m_pFile == NULL)
    {
        LOG(ERROR) << "Please open a file";
        return -1;
    }

    fseek (m_pFile , 0 , SEEK_END);
    return (int)ftell(m_pFile);
}

int BaseFile::fileRead(void *buffer, int size)
{
    if (m_pFile == NULL)
    {
        LOG(ERROR) << "Please open a file";
        return -1;
    }

    if (buffer == NULL)
    {
        LOG(ERROR) << "buffer is NULL";
        return -1;
    }

    //Sets the position indicator associated with stream to the beginning of the file.
    rewind(m_pFile);

    int ret = -1;
    ret = fread(buffer, 1, size, m_pFile);
    if (ret != size)
    {
        LOG(ERROR) << "Reading error, ret is not equal to the size";
        return -1;
    }

    return ret;
}

int BaseFile::fileWrite(const char *data, int size)
{
    if (m_pFile == NULL)
    {
        LOG(ERROR) << "Please open a file";
        return -1;
    }

    if (data == NULL)
    {
        LOG(ERROR) << "data is NULL";
        return -1;
    }

    int ret = -1;
    ret = fwrite(data, 1, size, m_pFile);
    if (ret != size)
    {
        LOG(ERROR) << "Reading error, ret is not equal to the size";
        return -1;
    }

    return ret;
}

FILE *BaseFile::getFilePointer()
{
    return m_pFile;
}

bool BaseFile::fileClose()
{
    if (m_pFile != NULL)
    {
        int ret;
        ret = fclose(m_pFile);
        if (ret != 0)
        {
            LOG(ERROR) << "fclose failure!";
            return false;
        }

        m_pFile = NULL;
    }

    return true;
}

int BaseFile::fileRemove(const char *filename)
{
    int ret = 0;

    if (filename)
    {
        ret = remove(filename);
    }

    return ret;
}

int BaseFile::fileSeek(int offset, int origin)
{
    if (m_pFile == NULL)
    {
        LOG(ERROR) << "Please open a file";
        return -1;
    }

    int ret = fseek(m_pFile, offset, origin);
    if (ret != 0)
    {
        LOG(ERROR) << "fseek error, return " << ret;
    }

    return ret;
}



