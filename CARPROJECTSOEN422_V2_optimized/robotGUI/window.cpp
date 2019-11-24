#include "window.h"
#include <QPushButton>
#include <QDebug>
#include <QKeyEvent>


Window::Window(QWidget *parent) : QWidget(parent) {
    // Set size of the window
    setFixedSize(300, 250);

    // Create car controller
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


//    Two wheels
    if (keys[Qt::Key_Up] && keys[Qt::Key_Left]) {
        car->rightForward();
    } else if (keys[Qt::Key_Down] && keys[Qt::Key_Left]) {
        car->rightBackward();
    } else if (keys[Qt::Key_O]) {
        car->rightStop();
    }
    else if (keys[Qt::Key_Up] && keys[Qt::Key_Right]) {
        car->leftForward();
    }  else if (keys[Qt::Key_Down] && keys[Qt::Key_Right]) {
        car->leftBackward();
    } else if (keys[Qt::Key_L]) {
        car->leftStop();
    }

//    All Wheels
    else if (keys[Qt::Key_Up]) {
        car->allForward();
    } else if (keys[Qt::Key_Down]) {
        car->allBackward();
    } else if (keys[Qt::Key_X]) {
        car->allStop();
    }

}
