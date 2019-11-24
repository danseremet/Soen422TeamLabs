// Client side
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <vector>
#include <QDebug>
#include "client.h"

#define PORT 8080
#define SERVER_IPV4 "10.42.0.134"

Client::Client() {
    sockid = 0;
    struct sockaddr_in serv_addr;
    std::string ipv4_addr{SERVER_IPV4};
    try {

        if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw "\n Socket creation error \n";
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, ipv4_addr.c_str(), &serv_addr.sin_addr) <= 0) {
            throw "\nInvalid address/ Address not supported \n";
        }

        if (connect(sockid, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            throw "\nConnection Failed \n";
        }

    } catch (std::string param) {
        qDebug() << param.c_str();
    }
}

void Client::sendMessage(const char* message) {
    using namespace std::this_thread;
    using namespace std::chrono;

    qDebug() << "Sending: " << message;
    send(sockid, message, strlen(message), 0);
    sleep_for(25ms);
}

Client::~Client() {
    close(sockid);
    qDebug() << "Socket closed.";
}
