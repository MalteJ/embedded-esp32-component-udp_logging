UDP Logging for esp32
=====================

How To
------

Just include the component into your project, include the header file and call udp_logging_init()

    #include "udp_logging.h"
    
    udp_logging_init( CONFIG_LOG_UDP_IP, CONFIG_LOG_UDP_PORT, udp_logging_vprintf );

On the server execute the logging_server.py file using Python v3:

    $ python3 logging_server.py



License
-------

This code is licensed under the Apache License v2.0
