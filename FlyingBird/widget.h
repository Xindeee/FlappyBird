#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "game.h"

class Game;

//! [0]

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent, Game * gameIn);
    void doFly();

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap pmBackground;
    QPixmap pmBird;
    int elapsed;
    Game * game;
};
//! [0]

#endif
