#include "widget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

//! [0]
Widget::Widget(QWidget *parent, Game * gameIn): QWidget(parent)
{
    game = gameIn;
    elapsed = 0;
    setFixedSize(800, 600);
    pmBackground = QPixmap(":/resource/cloud.png");
    pmBird = QPixmap(":/resource/bird2.png");
}
//! [0]

//! reDrawing form
void Widget::animate()
{
    game->doStep();
    update();
}
//!

//! [2]
void Widget::paintEvent(QPaintEvent *event)
{
    std::list<std::pair<Pipe, Pipe>> & pipes = game->getPipes();
    //animate by game objects
    QPainter painter;
    painter.begin(this);
    QPoint birdPosition = game->getBirdPosition();
    painter.drawPixmap(0,0,pmBackground);
    painter.drawPixmap(birdPosition.x(),birdPosition.y(),pmBird);

    for (auto iter = pipes.begin(); iter != pipes.end(); iter++){
        painter.fillRect(iter->first.getRect(), Qt::red);
        painter.fillRect(iter->second.getRect(), Qt::red);
    }

    //painter.fillRect(event->rect(), Qt::red);
    //painter.drawEllipse(QRectF(50, 10+elapsed/20, 50, 50));
    //painter.drawRect(QRectF(130, 10+elapsed/20, 50, 50));
    QWidget::paintEvent(event);
    painter.end();
}
//! [2]
void Widget::doFly(){
    game->spaceClicked();
}
