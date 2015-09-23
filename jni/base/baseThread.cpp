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

#include "baseThread.h"
#include <pthread.h>

baseThread::baseThread(void)
{

}

baseThread::~baseThread(void)
{

}

int baseThread::startThread(thread_func_t func, void *param)
{
    int ret;
    pthread_t thread_id;
    pthread_attr_t attr;

    (void)pthread_attr_init(&attr);
    (void)pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&thread_id, &attr, func, param);
    pthread_attr_destroy(&attr);

    return ret;
}

int baseThread::mutexInit(pthread_mutex_t *mutex, const void *attr)
{
    return pthread_mutex_init(mutex, (pthread_mutexattr_t*)attr);
}

int baseThread::mutexDestroy(pthread_mutex_t *mutex)
{
    return pthread_mutex_destroy(mutex);
}

int baseThread::mutexLock(pthread_mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}

int baseThread::mutexUnlock(pthread_mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}



