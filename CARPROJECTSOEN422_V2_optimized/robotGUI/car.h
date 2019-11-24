#ifndef CAR_H
#define CAR_H
#include "client.h"

class Car
{
public:
    Car();

//    Two Wheels
    void leftForward();
    void leftBackward();
    void leftStop();

    void rightForward();
    void rightBackward();
    void rightStop();

//    All Wheels
    void allForward();
    void allBackward();
    void allStop();
private:
    Client *client;
};

#endif // CAR_H
