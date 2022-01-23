#include "widget.h"
#include "window.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>

//! [0]
Window::Window()
{
    game = new Game(800, 600);
    mainWidget = new Widget(this, game);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(mainWidget, 0, 0);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, mainWidget, &Widget::animate);
    timer->start(20);
}
//! [0]
void Window::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Space)
    {
        mainWidget->doFly();
    }
    update();
}
