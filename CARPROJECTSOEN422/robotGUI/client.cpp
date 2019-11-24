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
#include "client.h"
#include <QDebug>


#define PORT 8080


Client::Client(std::string server_ip) {
    sockid = 0;
    struct sockaddr_in serv_addr;
    try {

        if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw "\n Socket creation error \n";
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
            throw "\nInvalid address/ Address not supported \n";
        }

        if (connect(sockid, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            throw "\nConnection Failed \n";
        }

    } catch (std::string param) {
        qDebug() << param.c_str();
    }
}

void Client::sendMessages(std::vector<std::string> messages) {
    using namespace std::this_thread;
    using namespace std::chrono;

    for(auto&& message: messages) {
        qDebug() << "Sending: " << message.c_str();
        const char * m = message.c_str();
        send(sockid, m, strlen(m), 0);
        sleep_for(25ms);
    }
}

Client::~Client() {
    close(sockid);
    qDebug() << "Socket closed.";
}
