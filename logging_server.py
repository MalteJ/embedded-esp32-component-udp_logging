#!/usr/bin/env python3
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
