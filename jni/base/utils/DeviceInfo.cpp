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
#include "base/utils/log.h"

#define MAX_INTERFACES  8

std::mutex mtx;
static int g_netType = 1;  //0-Ethernet, 1-Wireless, default

int setNetType(int type)
{
    g_netType = type;

    return 0;
}

//type: 0 eth0, 1 wlan0
std::string getMac(int type)
{
    mtx.lock();

    FILE *fp = NULL;
    char buffer[80] = {0};

    if (type == 0)
    {
        fp = popen("cat /sys/class/net/eth0/address","r");
    }
    else if (type == 1)
    {
        fp = popen("cat /sys/class/net/wlan0/address","r");
    }
    else
    {
        fp = popen("cat /sys/class/net/wlan0/address","r");
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

    mtx.unlock();

    LOG(DEBUG) << "getMac return: " << std::string(buffer);
    return std::string(buffer);
}

std::string getMacByFile()
{
    std::string mac;

#ifdef WLAN_MAC
    mac = getMac(1);
#endif

#ifdef ETH_MAC
    mac = getMac(0);
#endif

#ifdef SET_MAC
    mac = getMac(g_netType);
#endif

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
        LOG(ERROR) << "socket() failed";
        return wlan0_mac;
    }

    ifc.ifc_len = sizeof(ifreq_buf);
    ifc.ifc_buf = (caddr_t)ifreq_buf;

    if (!ioctl (sock, SIOCGIFCONF, (char *)&ifc))
    {
        interface = ifc.ifc_len / sizeof(struct ifreq);
        LOG(DEBUG) << "interface=" << interface;

        while (interface-- > 0)
        {
            LOG(DEBUG) << "ifr_name=" << ifreq_buf[interface].ifr_name;

            //get the MAC of this net device
            if (!(ioctl(sock, SIOCGIFHWADDR, (char *)&ifreq_buf[interface])))
            {
                p = ifreq_buf[interface].ifr_hwaddr.sa_data;

                sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", \
                    (unsigned char)p[0], (unsigned char)p[1], \
                    (unsigned char)p[2], (unsigned char)p[3], \
                    (unsigned char)p[4], (unsigned char)p[5]);

                LOG(DEBUG) << ifreq_buf[interface].ifr_name << "MAC=" << mac;

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

    LOG(DEBUG) << "getMacInNum : " << mac;

    return mac;
}


