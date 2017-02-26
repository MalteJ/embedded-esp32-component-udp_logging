#!/usr/bin/env python3

#  Copyright 2017 by Malte Janduda
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.


import socket
import datetime

UDP_IP = "0.0.0.0"
UDP_PORT = 1337

sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
sock.bind( (UDP_IP, UDP_PORT) )

print("+============================+")
print("|  ESP32 UDP Logging Server  |")
print("+============================+")
print("")

while True:
	data, addr = sock.recvfrom(1024)
	print(datetime.datetime.now(), data.decode(), end='')
