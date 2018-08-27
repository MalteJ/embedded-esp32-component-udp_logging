//  Copyright 2017 by Malte Janduda
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "udp_logging.h"

#include "esp_system.h"
#include "esp_log.h"

#include <string.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

int udp_log_fd;
static struct sockaddr_in serveraddr;
static uint8_t buf[UDP_LOGGING_MAX_PAYLOAD_LEN];
static uint32_t len;

void udp_logging_free() {
    esp_log_set_vprintf(vprintf);
    shutdown(udp_log_fd, 0);
    close( udp_log_fd );
    udp_log_fd = 0;
}

static int udp_logging_vprintf( const char *str, va_list l ) {
    int err = 0;
	len = vsprintf((char*)buf, str, l);
    if( (err = sendto(udp_log_fd, buf, len, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0 )
    {
    	vprintf("\nFreeing UDP Logging. sendto failed!\n", l);
    	udp_logging_free();
    	return vprintf("UDP Logging freed!\n\n", l);
    }
	return vprintf( str, l );
}

int udp_logging_init(const char *ipaddr, unsigned long port) {
	struct timeval send_timeout = {1,0};
	udp_log_fd = 0;
	ESP_LOGI("UDP_LOGGING", "initializing udp logging...");
    if( (udp_log_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
       ESP_LOGE("UDP_LOGGING", "Cannot open socket!");
       return -1;
    }

    uint32_t ip_addr_bytes;
    inet_aton(ipaddr, &ip_addr_bytes);
    ESP_LOGI("UDP_LOGGING", "Logging to 0x%x", ip_addr_bytes);

    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( port );
    serveraddr.sin_addr.s_addr = ip_addr_bytes;

    int err = setsockopt(udp_log_fd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&send_timeout, sizeof(send_timeout));
	if (err < 0) {
	   ESP_LOGE("UDP_LOGGING", "Failed to set SO_SNDTIMEO. Error %d", err);
	}

    esp_log_set_vprintf(udp_logging_vprintf);

    return 0;
}

