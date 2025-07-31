# UDP Logging for esp32

## How To

Just include the component into your project, include the header file and call udp_logging_init()

    #include "udp_logging.h"

    udp_logging_init( "myserver", "3333", udp_logging_vprintf );

On the server execute the logging_server.py file using Python v3:

    $ python3 logging_server.py

## License

This code is licensed under the Apache License v2.0
