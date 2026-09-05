import socket

TARGET_IP = "127.0.0.1"
TARGET_PORT = 9000
MESSAGE = "Hello World"

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sender:
    bytes_sent = sender.sendto(MESSAGE.encode("utf-8"), (TARGET_IP, TARGET_PORT))

print(f"Sent {bytes_sent} bytes to {TARGET_IP}:{TARGET_PORT}: {MESSAGE}")