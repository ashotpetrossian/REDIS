#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

static const int bufSIZE = 255;

int main(int argc, char* argv[]) {

    int sockfd, n, portno;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[bufSIZE];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "!!!Error opening socket!!!\n";
    }

    std::cout << ":: Socket created..." << std::endl;

    portno = std::atoi(argv[1]);

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // IPv4 Picsart Academy
    serv_addr.sin_addr.s_addr = inet_addr("192.168.11.208");
    serv_addr.sin_port = htons(portno);
    // trying to connect
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "!!!Connection failed!!!\n";
    }

    std::cout << ":: Connection confirmed...\n";
        
    while (1) {
        bzero(buffer, bufSIZE);
        fgets(buffer, bufSIZE, stdin);

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            std::cout << "!!!Error on writing!!!\n";
        }
        if (buffer[0] == 'e' && buffer[1] == 'x' &&
            buffer[2] == 'i' && buffer[3] == 't' &&
            strlen(buffer) == 5) {            
            break;
        }

        bzero(buffer, bufSIZE);
        n = read(sockfd, buffer, bufSIZE);
        if (n < 0) {
            std::cout << "!!!Error on reading!!!\n";
        }
        std::cout << ":: Server -> \n" << buffer << std::endl;
    }

    close(sockfd);
    return 0;
}