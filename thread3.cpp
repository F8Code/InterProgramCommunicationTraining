#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

const int TCP_PORT = 8081;
const int UDP_PORT = 8082;
const char* SERVER_IP = "127.0.0.1";

int main() {
    // Tworzenie gniazda TCP
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket == -1) {
        std::cerr << "TCP Socket creation error! Closing program...";
        return 1;
    }

    // Konfigurowanie adresu gniazda TCP
    struct sockaddr_in tcpServerAddr;
    tcpServerAddr.sin_family = AF_INET;
    tcpServerAddr.sin_addr.s_addr = INADDR_ANY;
    tcpServerAddr.sin_port = htons(TCP_PORT);

    // Bindowanie gniazda TCP do adresu i portu
    if(bind(serverSocket, (struct sockaddr *)&tcpServerAddr, sizeof(tcpServerAddr)) == -1) {
        std::cerr << "TCP Socket binding error! Closing program...";
        close(serverSocket);
        return 1;
    }

    // Nasłuchiwanie połączenia z gniazdem TCP klienta
    if(listen(serverSocket, 5) == -1) {
        std::cerr << "TCP Socket listening error! Closing program...";
        close(serverSocket);
        return 1;
    }

    // Akceptowanie gniazda TCP klienta
    int clientSocket = accept(serverSocket, NULL, NULL);
    if(clientSocket == -1) {
        std::cerr << "TCP Connection failed! Closing program...";
        close(serverSocket);
        return 1;
    }

    // Tworzenie gniazda UDP
    int udpClientSocket;
    struct sockaddr_in udpServerAddr;
    socklen_t addrSize = sizeof(udpServerAddr);

    if ((udpClientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "UDP Socket creation failed" << std::endl;
        close(clientSocket);
        close(serverSocket);
        return -1;
    }

    // Konfigurowanie adresu gniazda UDP
    memset((char *)&udpServerAddr, 0, sizeof(udpServerAddr));
    udpServerAddr.sin_family = AF_INET;
    udpServerAddr.sin_port = htons(UDP_PORT);
    udpServerAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    char buffer[1024];

    while (true) {
        // Otrzymywanie wiadomości z gniazda TCP
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "TCP Receive failed! Closing program...";
            break;
        } else {
            // Wysyłanie otrzymanych wiadomości gniazdem UDP
            if (sendto(udpClientSocket, buffer, bytesReceived, 0, (struct sockaddr *)&udpServerAddr, addrSize) < 0) {
                std::cerr << "UDP Sendto failed" << std::endl;
                close(clientSocket);
                close(serverSocket);
                close(udpClientSocket);
                return -1;
            }
            
            //Czyszczenie buffera danych
			memset(buffer, 0, sizeof(buffer));
        }
    }

    // Zamykanie gniazd
    close(clientSocket);
    close(serverSocket);
    close(udpClientSocket);

    return 0;
}
