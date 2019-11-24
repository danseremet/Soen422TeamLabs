#include "car.h"
#include "client.h"

Car::Car() {
    client = new Client();
}

//All Wheels

void Car::allForward() {
    client->sendMessage("B");
}

void Car::allBackward() {
    client->sendMessage("N");
}

void Car::allStop() {
    client->sendMessage("X");
}

//Two Wheels

void Car::leftForward() {
    client->sendMessage("U");
}

void Car::leftBackward() {
    client->sendMessage("I");
}

void Car::leftStop() {
    client->sendMessage("O");
}

void Car::rightForward() {
    client->sendMessage("J");
}

void Car::rightBackward() {
    client->sendMessage("K");
}

void Car::rightStop() {
    client->sendMessage("L");
}
