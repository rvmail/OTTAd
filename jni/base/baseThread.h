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
*/

#ifndef __BASETHREAD_H__
#define __BASETHREAD_H__

#include <pthread.h>

typedef void *(*thread_func_t)(void *);

class baseThread
{
public:
    baseThread(void);
    ~baseThread(void);

    static int startThread(thread_func_t func, void *param);
    static int mutexInit(pthread_mutex_t *mutex, const void *attr);
    static int mutexDestroy(pthread_mutex_t *mutex);
    static int mutexLock(pthread_mutex_t *mutex);
    static int mutexUnlock(pthread_mutex_t *mutex);
private:

};

#endif
