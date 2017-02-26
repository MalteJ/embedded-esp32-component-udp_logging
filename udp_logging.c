#include "udp_logging.h"

#include "esp_system.h"
#include "esp_log.h"

#include <string.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#define MAX_PAYLOAD_LEN 1024
#define SERVER_IP "192.168.0.108"
#define SERVER_PORT 1337

static int fd;
static struct sockaddr_in serveraddr;
static uint8_t buf[MAX_PAYLOAD_LEN];
static uint32_t len;

static int udp_logging_vprintf( const char *str, va_list l ) {
    len = vsprintf((char*)buf, str, l);
    sendto(fd, buf, len, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    return vprintf( str, l );
}

int udp_logging_init(const char *ipaddr, unsigned long port ) {
    ESP_LOGI("UDP_LOGGING", "initializing udp logging...");
    if( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
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

    esp_log_set_vprintf(udp_logging_vprintf);

    return 0;
}

void udp_logging_free() {
    esp_log_set_vprintf(vprintf);
    close( fd );
}

