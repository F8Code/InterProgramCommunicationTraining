#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8082
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    ssize_t valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Tworzenie gniazda UDP
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Konfigurowanie adresu gniazda UDP
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bindowanie gniazda UDP do adresu i portu
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Odbieranie danych od klienta
        valread = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (valread < 0)
 		{
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }
        else if (valread > 0) {
			// Wypisywanie treści wiadomości
			for (int i = 0; i < valread; ++i) {
				printf("%02X", buffer[i]);
			}
			printf("\n");
			
			//Czyszczenie buffera danych
			memset(buffer, 0, BUFFER_SIZE);
		}
    }
    
    return 0;
}
