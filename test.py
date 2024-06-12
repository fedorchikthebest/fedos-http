import socket

sock = socket.socket()
sock.connect(("127.0.0.1", 5000))
sock.send(b"TEST")