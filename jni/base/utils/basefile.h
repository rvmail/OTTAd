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

#ifndef __BASEFILE_H__
#define __BASEFILE_H__

#include <stdio.h>

class BaseFile
{
public:
    /**
    * Constructs a new BaseFile.
    */
    BaseFile();

    /**
    * Destrcuctor.
    */
    ~BaseFile();

    /**
    * Check whether a file exist.
    * @param filename The name of the file.
    * @return True if the given file is exist, false otherwise.
    */
    bool isExist(const char *filename);

    /**
    * Open the file whose name specified in the parameter filename and
    * associates it with a stream.
    * @param filename The name of the file.
    * @param mode A file access mode.
    * @return true success, false on failure.
    */
    bool fileOpen(const char *filename, const char *mode);

    /**
    * Get the size of the file.
    * @return The size.
    */
    int getSize();

    /**
    * Read data from the stream and stores them in the block of memory
    * specified by buffer.
    * @param buffer Pointer to a block of memory with a size of at least (size) bytes.
    * @param size Size, in bytes, to be read.
    * @return The size have been read, -1 on failure.
    */
    int fileRead(void *buffer, int size);

    int fileWrite(const char *data, int size);

    /**
    * Get the pointer to the stream.
    * @return Pointer to the stream.
    */
    FILE *getFilePointer();

    /**
    * Close the file associated with the stream and disassociates it.
    * @return True success, false otherwise.
    */
    bool fileClose();

    //Deletes the file whose name is specified in filename.
    int fileRemove(const char *filename);

    int fileSeek(int offset, int origin);

private:
    //Pointer to a FILE object that identifies the stream.
    FILE *m_pFile;
};

#endif
