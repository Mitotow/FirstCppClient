#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <arpa/inet.h>
#include <cstring>

#define PORT 8081
#define MAX_TRIES 10

int main() {
    int sockfd, tries;
    struct sockaddr_in serv_addr{};
    char const *client_message = "Hello World! From client.";
    char buffer[1024] = { 0 };

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("address convertion failed --> invalid or not supported");
        exit(EXIT_FAILURE);
    }

    tries = 1;
    while(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        if(tries > MAX_TRIES) {
            perror("Impossible to establish connection from client to server.");
            exit(EXIT_FAILURE);
        }
        std::cout << "Try to establish connection to server ... Try. " << tries  << std::endl;
        tries++;
        sleep(1);
    }

    send(sockfd, client_message, strlen(client_message), 0);
    std::cout << "Client message sent." << std::endl;
    read(sockfd, buffer, 1024);
    std::cout << buffer << std::endl;

    close(sockfd);
    exit(EXIT_SUCCESS);
}