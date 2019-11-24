#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMap>
#include "car.h"

class QPushButton;
class Window : public QWidget
{
public:
    explicit Window(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *e);
    void timerEvent(QTimerEvent *);
private:
    QPushButton *m_button;
    QMap<int, bool> keys;

    Car *car;
};

#endif // WINDOW_H
