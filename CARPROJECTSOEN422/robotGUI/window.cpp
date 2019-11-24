#include "window.h"
#include <QPushButton>
#include <QDebug>
#include <QKeyEvent>

#define SERVER_IPV4 "10.42.0.134"   // try it later

Window::Window(QWidget *parent) : QWidget(parent) {
    // Set size of the window
    setFixedSize(300, 250);

    // Create client connection
    client = new Client(SERVER_IPV4);
    car = new Car();

    // Create and position the button
    m_button = new QPushButton("Hello World", this);
    m_button->setGeometry(10, 10, 80, 30);

    setFocusPolicy(Qt::StrongFocus);
    startTimer(1000/60);
}

void Window::keyPressEvent(QKeyEvent *e) {
    keys[e->key()] = true;
    QWidget::keyPressEvent(e);
}

void Window::keyReleaseEvent(QKeyEvent *e) {
    keys[e->key()] = false;
    QWidget::keyReleaseEvent(e);
}

void Window::timerEvent(QTimerEvent *) {

    // Not handling multiple keys for the moment, but we could.
    // Not trying to implement car physics this is only proof of concept.

    // e.g. meaning L F f 255 = Left motor, Forward, front wheel, 255
    if (keys[Qt::Key_Up]) {
        qDebug() << "UP";
        car->goForward(true);
        qDebug() << "set forward true";
        std::vector<std::string> m{ "L F f 255", "L F b 255", "R F f 255", "R F b 255"};
        client->sendMessages(m);
    }
    if (keys[Qt::Key_Down]) {
        qDebug() << "DOWN";
        car->goForward(false);
        qDebug() << "set forward false";
        std::vector<std::string> m{ "L B f 255", "L B b 255", "R B f 255", "R B b 255"};
        client->sendMessages(m);
    }
    if (keys[Qt::Key_Left]) {
        qDebug() << "LEFT";
        if (car->isGoingForward()) {
            std::vector<std::string> m{ "L F f 0", "L F b 0", "R F f 255", "R F b 255"};
            client->sendMessages(m);
        } else {
            std::vector<std::string> m{ "L B f 0", "L B b 0", "R B f 255", "R B b 255"};
            client->sendMessages(m);
        }
    }
    if (keys[Qt::Key_Right]) {
        qDebug() << "RIGHT";
        if (car->isGoingForward()) {
            std::vector<std::string> m{ "L F f 255", "L F b 255", "R F f 0", "R F b 0"};
            client->sendMessages(m);
        } else {
            std::vector<std::string> m{ "L B f 255", "L B b 255", "R B f 0", "R B b 0"};
            client->sendMessages(m);
        }
    }

    ////// TEST wheels directions speed 255

    // LEFT SIDE
    // TOP motor forward
    if (keys[Qt::Key_Q]) {
        qDebug() << "leftFrontWheelForward";
        std::vector<std::string> leftFrontWheelForward{ "L F f 255"};
        client->sendMessages(leftFrontWheelForward);
    }
    // TOP motor backwards
    if (keys[Qt::Key_W]) {
        qDebug() << "leftFrontWheelBackward";
        std::vector<std::string> leftFrontWheelBackward{ "L B f 255"};
        client->sendMessages(leftFrontWheelBackward);
    }
    // BOTTOM motor forward
    if (keys[Qt::Key_A]) {
        qDebug() << "leftBackWheelForward";
        std::vector<std::string> leftBackWheelForward{ "L F b 255"};
        client->sendMessages(leftBackWheelForward);
    }
    // BOTTOM motor backwards
    if (keys[Qt::Key_S]) {
        qDebug() << "leftBackWheelBackward";
        std::vector<std::string> leftBackWheelBackward{ "L B b 255"};
        client->sendMessages(leftBackWheelBackward);
    }

    // RIGHT SIDE
    // TOP motor forward
    if (keys[Qt::Key_E]) {
        qDebug() << "rightFrontWheelForward";
        std::vector<std::string> rightFrontWheelForward{ "R F f 255"};
        client->sendMessages(rightFrontWheelForward);
    }
    // TOP motor backwards
    if (keys[Qt::Key_R]) {
        qDebug() << "rightFrontWheelBackward";
        std::vector<std::string> rightFrontWheelBackward{ "R B f 255"};
        client->sendMessages(rightFrontWheelBackward);
    }
    // BOTTOM motor forward
    if (keys[Qt::Key_D]) {
        qDebug() << "rightBackWheelForward";
        std::vector<std::string> rightBackWheelForward{ "R F b 255"};
        client->sendMessages(rightBackWheelForward);
    }
    // BOTTOM motor backwards
    if (keys[Qt::Key_F]) {
        qDebug() << "rightBackWheelBackward";
        std::vector<std::string> rightBackWheelBackward{ "R B b 255"};
        client->sendMessages(rightBackWheelBackward);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// TEST wheels directions speed 0
    // LEFT SIDE
    // TOP motor forward
    if (keys[Qt::Key_T]) {
        qDebug() << "leftFrontWheelForward";
        std::vector<std::string> leftFrontWheelForward{ "L F f 0"};
        client->sendMessages(leftFrontWheelForward);
    }
    // TOP motor backwards
    if (keys[Qt::Key_Y]) {
        qDebug() << "leftFrontWheelBackward";
        std::vector<std::string> leftFrontWheelBackward{ "L B f 0"};
        client->sendMessages(leftFrontWheelBackward);
    }
    // BOTTOM motor forward
    if (keys[Qt::Key_G]) {
        qDebug() << "leftBackWheelForward";
        std::vector<std::string> leftBackWheelForward{ "L F b 0"};
        client->sendMessages(leftBackWheelForward);
    }
    // BOTTOM motor backwards
    if (keys[Qt::Key_H]) {
        qDebug() << "leftBackWheelBackward";
        std::vector<std::string> leftBackWheelBackward{ "L B b 0"};
        client->sendMessages(leftBackWheelBackward);
    }

    // RIGHT SIDE
    // TOP motor forward
    if (keys[Qt::Key_U]) {
        qDebug() << "rightFrontWheelForward";
        std::vector<std::string> rightFrontWheelForward{ "R F f 0"};
        client->sendMessages(rightFrontWheelForward);
    }
    // TOP motor backwards
    if (keys[Qt::Key_I]) {
        qDebug() << "rightFrontWheelBackward";
        std::vector<std::string> rightFrontWheelBackward{ "R B f 0"};
        client->sendMessages(rightFrontWheelBackward);
    }
    // BOTTOM motor forward
    if (keys[Qt::Key_J]) {
        qDebug() << "rightBackWheelForward";
        std::vector<std::string> rightBackWheelForward{ "R F b 0"};
        client->sendMessages(rightBackWheelForward);
    }
    // BOTTOM motor backwards
    if (keys[Qt::Key_K]) {
        qDebug() << "rightBackWheelBackward";
        std::vector<std::string> rightBackWheelBackward{ "R B b 0"};
        client->sendMessages(rightBackWheelBackward);
    }

    // STOP ALL
    if (keys[Qt::Key_X]) {
        qDebug() << "stop all";
        std::vector<std::string> stopAll{ "L B f 0", "L B b 0", "R B f 0", "R B b 0"};
        client->sendMessages(stopAll);
    }
}
