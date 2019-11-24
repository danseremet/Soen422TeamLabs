#ifndef CAR_H
#define CAR_H


class Car
{
public:
    Car();
    bool isGoingForward() {
        return forward;
    }
    void goForward(bool f) {
        forward = f;
    }
private:
    bool forward;
};

#endif // CAR_H
