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
*/

#include <jni.h>

#include "ottlogin.h"
#include "base/utils/log.h"

#define CLASS_MAINACTIVITY "tv/icntv/ottlogin/MainActivity"

#define TRANSFER_DATA "tv/icntv/ottlogin/TransferData"

extern int setNetworkType(int type);

#ifdef __cplusplus
extern "C" {
#endif

JavaVM* gVM = NULL;

JNIEnv* GetEnv()
{
   JNIEnv* env = NULL;
   if (gVM) {
      jint ret = gVM->GetEnv((void**) &env, JNI_VERSION_1_6);
      if (ret != JNI_OK) {
         jint status = gVM->AttachCurrentThread(&env, NULL);
         if (status < 0) {
            LOG(ERROR) << "GetEnv: failed to attach current thread";
            env = NULL;
         }
      }
   }
   return env;
}

void DetachEnv()
{
   gVM->DetachCurrentThread();
}

void setResult(JNIEnv *env, jobject obj, const char *data)
{
    jclass transfer = env->FindClass(TRANSFER_DATA);
    jmethodID setResult;
    setResult = env->GetMethodID(transfer, "setResult", "(Ljava/lang/String;)V");
    jstring jstr = env->NewStringUTF(data);
    env->CallVoidMethod(obj, setResult, jstr);
    env->DeleteLocalRef(jstr);
}

void setLength(JNIEnv *env, jobject obj, int length)
{
    jclass transfer = env->FindClass(TRANSFER_DATA);
    jmethodID setlength;
    setlength = env->GetMethodID(transfer, "setLength", "(I)V");
    env->CallVoidMethod(obj, setlength, length);
}

static string jstringToString(JNIEnv *env, jstring jstr)
{
    char *p = NULL;
    string str;

    if (jstr == NULL)
    {
        LOG(ERROR) << "jstringToString: jstr == NULL";
        return str;
    }

    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize len = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (len > 0)
    {
        p = new char[len + 1];
        memcpy(p, ba, len);
        p[len] = 0;

        str.assign(p);
        delete [] p;
    }

    env->ReleaseByteArrayElements(barr,ba,0);

    return str;
 }

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    return JNI_VERSION_1_6;
}

JNIEXPORT jint JNICALL Java_tv_icntv_ottlogin_loginSDK_setNetworkType
  (JNIEnv *env, jobject thiz, jint type)
{
    setNetworkType(type);
    return 0;
}

JNIEXPORT jboolean JNICALL Java_tv_icntv_ottlogin_loginSDK_sdkInitialize
(JNIEnv *env, jobject thiz, jstring path)
{
    string currPath = jstringToString(env, path);

    return sdkInitialize(currPath);
}

// APP_CMD_DESTROY
JNIEXPORT jboolean JNICALL Java_tv_icntv_ottlogin_loginSDK_sdkExit
(JNIEnv *env, jobject thiz)
{
    return sdkExit();
}

JNIEXPORT jint JNICALL Java_tv_icntv_ottlogin_loginSDK_getVersion
(JNIEnv *env, jobject thiz, jobject data)
{
    int ret = -1;
    string str;

    ret = getVersion(str);
    if (ret != 0)
    {
        LOG(ERROR) << "JNI-getVersion() error";
        return -1;
    }

    setLength(env, data, str.length());
    setResult(env, data, str.c_str());

    return 0;
}

JNIEXPORT jint JNICALL Java_tv_icntv_ottlogin_loginSDK_getDeviceID
(JNIEnv *env, jobject thiz, jobject data)
{
    int ret = -1;
    string str;

    ret = getDeviceID(str);
    if (ret != 0)
    {
        LOG(ERROR) << "JNI-getDeviceID() error";
        return -1;
    }

    setLength(env, data, str.length());
    setResult(env, data, str.c_str());

    return 0;
}

JNIEXPORT jint JNICALL Java_tv_icntv_ottlogin_loginSDK_getUserID
(JNIEnv *env, jobject thiz, jobject data)
{
    int ret = -1;
    string str;

    ret = getUserID(str);
    if (ret != 0)
    {
        LOG(ERROR) << "JNI-getUserID() error";
        return -1;
    }

    setLength(env, data, str.length());
    setResult(env, data, str.c_str());

    return 0;
}

JNIEXPORT jint JNICALL Java_tv_icntv_ottlogin_loginSDK_getTemplateID
(JNIEnv *env, jobject thiz, jobject data)
{
    int ret = -1;
    string str;

    ret = getTemplateID(str);
    if (ret != 0)
    {
        LOG(ERROR) << "JNI-getTemplateID() error";
        return -1;
    }

    setLength(env, data, str.length());
    setResult(env, data, str.c_str());

    return 0;
}

JNIEXPORT jint JNICALL Java_tv_icntv_ottlogin_loginSDK_getPlatformID
(JNIEnv *env, jobject thiz, jobject data)
{
    int ret = -1;
    string str;

    ret = getPlatformID(str);
    if (ret != 0)
    {
        LOG(ERROR) << "JNI-getPlatformID() error";
        return -1;
    }

    setLength(env, data, str.length());
    setResult(env, data, str.c_str());

    return 0;
}

JNIEXPORT jint JNICALL Java_tv_icntv_ottlogin_loginSDK_getServerAddress
(JNIEnv *env, jobject thiz, jstring type, jobject data)
{
    int ret = -1;
    string str;

    string ty = jstringToString(env, type);

    ret = getServerAddress(ty, str);
    if (ret != 0)
    {
        LOG(ERROR) << "JNI-getServerAddress() error";
        return -1;
    }

    setLength(env, data, str.length());
    setResult(env, data, str.c_str());

    return 0;
}

#ifdef __cplusplus
}
#endif
