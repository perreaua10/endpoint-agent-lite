# udp_reciever.py , easier to run for wsl reciever side
import socket
import sqlite3

con = sqlite3.connect("tutorial.db")
con.execute("CREATE TABLE IF NOT EXISTS processes (id INTEGER PRIMARY KEY, name TEXT, pid INTEGER)")
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server.bind(('0.0.0.0', 9999))  # match whatever port your C++ sender targets

print("Mock UDP server listening on 0.0.0.0:9999")

count = 0
while True:
    data, addr = server.recvfrom(65536)  # max UDP payload size to read at once
    count += 1
    print(f"Received request {count} from {addr[0]}:{addr[1]}: {data.decode('utf-8')}")
    con.execute("INSERT INTO processes (name, pid) VALUES (?, ?)", (data.decode('utf-8'), count))
    con.commit()