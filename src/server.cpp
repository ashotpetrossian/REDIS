#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "parser.h"

using namespace DBParser;

static const int bufSIZE = 255;
char buffer[bufSIZE];

void* socketThread(void* arg) {
    int newSocket = *((int*) arg);
    Parser parser;

    while (1) {
        bzero(buffer, bufSIZE);
        int n = read(newSocket, buffer, bufSIZE);

        if (n <= 0) {
            std::cout << "!!!Error on reading!!!\n";
        }

        if (buffer[0] == 'e' && buffer[1] == 'x' &&
            buffer[2] == 'i' && buffer[3] == 't' &&
            strlen(buffer) == 5) {            
            break;
        }

        parser.send_buffer(buffer);
        parser.manager();
        bzero(buffer, bufSIZE);
        parser.modify_buffer(buffer);

        n = write(newSocket, buffer, strlen(buffer));
        if (n < 0) {
            std::cout << "!!!Error on writing a message!!!\n";
        }
    }

    sleep(1);

    close(newSocket);
    pthread_exit(nullptr);
}

class Server {
public:
    Server() = default;
    Server(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(const Server&) = delete;
    Server& operator=(Server&&) = delete;
    ~Server() = default;

    void create_socket(char* argv[]) {
        // creating a socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) { 
            std::cout << "!!!Error on opening a Socket\n"; exit(1);
        }
        std::cout << ":: Socket was created...\n";
        // cleaning the struct sockaddr_in
        bzero((char*) &serv_addr, sizeof(serv_addr));
        
        portno = std::atoi(argv[1]);
        // Address family Internet 
        serv_addr.sin_family = AF_INET;
        // IPv4 (Picsart Academy)
        serv_addr.sin_addr.s_addr = inet_addr("192.168.11.208");
        // portnumber is setted by server
        serv_addr.sin_port = htons(portno);
    }
    
    void bind_listen_accept() {
        // checking if binding was successful
        if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
            std::cout << "!!!Binding Failed!!!\n"; exit(1);
        }
        // listening to clients
        int l = listen(sockfd, 5);
        if (l == 0) {
            std::cout << ":: Listening to a client...\n";
        } else {
            std::cout << "!!!Listening failed!!!\n"; exit(1);
        }

        pthread_t thread;
        int count{0}; // max 8 clients

        while (count < 8) {
            newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);

            if (newsockfd < 0) {
                std::cerr << "!!!Connection failed!!!\n";
            }

            std::cout << ":: Connection accepted...\n";

            if (pthread_create(&thread, nullptr, 
                    socketThread, &newsockfd) != 0) {
                        std::cout << "!!!Creating thread failed!!!\n";
                        exit(1);
            }
            ++count;

            if (newsockfd < 0) {
                std::cout << "!!!Error on accepting!!!" << std::endl;
            }
        }
    }

    void close_server_client() {
        close(newsockfd);
        close(sockfd);
    }
    
private:
    int sockfd;
    int newsockfd;
    int portno;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    socklen_t clilen;
};

int main(int argc, char* argv[])
{
    Server ser;
    ser.create_socket(argv);
    ser.bind_listen_accept();
    ser.close_server_client();
}