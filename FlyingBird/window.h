#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "widget.h"

//! [0]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Widget * mainWidget;
    Game * game;
};
//! [0]

#endif
