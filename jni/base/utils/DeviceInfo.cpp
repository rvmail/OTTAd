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
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <mutex>

#include "DeviceInfo.h"
#include "debug.h"
#include "dataCache.h"

#define MAX_INTERFACES  8

static int g_netType = 1;  //0-Ethernet, 1-Wireless, default

int setNetType(int type)
{
    g_netType = type;

    return 0;
}

static std::string getMacByFile(std::string macFile)
{
    FILE *fp = NULL;
    char buffer[80] = {0};

    if (macFile.empty())
    {
        LOGERROR("macFile is empty\n");
        return "";
    }

    std::string command("cat ");
    command += macFile;

    fp = popen(command.c_str(), "r");
    if (!fp)
    {
        LOGWARN("fp is NULL\n");
        return "";
    }

    while (fgets(buffer, sizeof(buffer), fp))
    {
        for (int i = 0; buffer[i]; i++)
        {
            if (buffer[i] == '\n' || buffer[i] == '\r')
            {
                buffer[i] = 0;
                break;
            }
        }
    }
    pclose(fp);

    std::string mac(buffer);
    LOGDEBUG("getMacByFile: MAC=%s\n", mac.c_str());
    return mac;
}

std::string getMac(int type, std::string macFile)
{
    std::string license = dataCache::getInstance()->getLicense();
    if (license != "")
    {
        LOGINFO("konka license is used: %s\n", license.c_str());
        return license;
    }

    std::string file("/sys/class/net/wlan0/address");

    if (type == 1)
    {
        file = "/sys/class/net/wlan0/address";
    }
    else if (type == 2)
    {
        file = "/sys/class/net/eth0/address";
    }
    else if (type == 3)
    {
        file = macFile;
    }

    if (file.empty())
    {
        LOGERROR("file is empty\n");
        return "";
    }

    std::string mac = getMacByFile(file);
    return mac;
}

std::string getMacBySocket()
{
    int sock, interface;
    struct ifreq ifreq_buf[MAX_INTERFACES];
    struct ifconf ifc;
    char mac[7] = {0};
    char *p = NULL;
    std::string wlan0_mac, eth0_mac;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        LOGERROR("socket() failed\n");
        return wlan0_mac;
    }

    ifc.ifc_len = sizeof(ifreq_buf);
    ifc.ifc_buf = (caddr_t)ifreq_buf;

    if (!ioctl (sock, SIOCGIFCONF, (char *)&ifc))
    {
        interface = ifc.ifc_len / sizeof(struct ifreq);
        LOGDEBUG("interface=%d\n", interface);

        while (interface-- > 0)
        {
            LOGDEBUG("ifr_name=%s\n", ifreq_buf[interface].ifr_name);

            //get the MAC of this net device
            if (!(ioctl(sock, SIOCGIFHWADDR, (char *)&ifreq_buf[interface])))
            {
                p = ifreq_buf[interface].ifr_hwaddr.sa_data;

                sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", \
                    (unsigned char)p[0], (unsigned char)p[1], \
                    (unsigned char)p[2], (unsigned char)p[3], \
                    (unsigned char)p[4], (unsigned char)p[5]);

                LOGDEBUG("MAC=%s\n", mac);

                if (strcmp(ifreq_buf[interface].ifr_name, "wlan0") == 0)
                {
                    wlan0_mac.assign(mac);
                }
                else if (strcmp(ifreq_buf[interface].ifr_name, "eth0") == 0)
                {
                    eth0_mac.assign(mac);
                }
            }
        }
    }

    close(sock);

    if (eth0_mac.length() != 0)
    {
        return eth0_mac;
    }

    return wlan0_mac;

}

unsigned long long  getMacInNum()
{
    std::string data = getMacBySocket();
    unsigned int tmp[6] = {0};
    unsigned long long mac = 0, m = 0;
    int rv;

    rv = sscanf(data.c_str(), "%2x:%2x:%2x:%2x:%2x:%2x", &tmp[0], &tmp[1], &tmp[2],
            &tmp[3], &tmp[4], &tmp[5]);

    for (int i = 0; i < 6; i++)
    {
        m = tmp[i];
        mac += (m << (8*(6-1-i)));
    }

    LOGDEBUG("mac=%lld\n", mac);
    return mac;
}

unsigned long long convertMac2Num(std::string mac)
{
    int i;
    int ret;
    unsigned int tmp[6] = {0};
    unsigned long long num = 0, m = 0;

    ret = sscanf(mac.c_str(), "%2x:%2x:%2x:%2x:%2x:%2x", &tmp[0], \
                 &tmp[1], &tmp[2], &tmp[3], &tmp[4], &tmp[5]);

    for (i = 0; i < 6; i++)
    {
        m = tmp[i];
        num += (m << (8*(6-1-i)));
    }

    LOGDEBUG("macnum=%lld\n", num);

    return num;
}


