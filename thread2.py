import socket
import sys
from http.server import BaseHTTPRequestHandler, HTTPServer
from threading import Thread

class RequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)

        # Przeforwardowanie otrzymanych danych do gniazda TCP
        try:
            # Send data through the open TCP socket
            tcp_socket.sendall(post_data.decode('utf-8').split('=', 1)[1].encode())
        except (socket.error, OSError) as e:
            print("Error sending data through TCP socket:", e)

if __name__ == '__main__':
    # Połączenie z gniazdem TCP
    server_address = ('localhost', 8081)
    try:
        tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tcp_socket.connect(server_address)
    except (socket.error, OSError) as e:
        print("Error connecting to TCP server:", e)
        sys.exit(1)

    server_address = ('localhost', 8080)
    httpd = HTTPServer(server_address, RequestHandler)
    httpd.serve_forever()
