#ifndef CLIENT_H
#define CLIENT_H


class Client {
public:
    Client();
    ~Client();

    void sendMessage(const char* message);

private:
    int sockid;
};


#endif // CLIENT_H
