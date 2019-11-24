#ifndef CLIENT_H
#define CLIENT_H


class Client {
public:
    Client(std::string server_ip);
    ~Client();
    void sendMessages(std::vector<std::string> messages);
private:
    int sockid;
};


#endif // CLIENT_H
