#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <list>
#include <cmath>
#include <random>
#include <utility>

#include <QRect>
#include <QDateTime>

#include "widget.h"

class Widget;

const double Pi = 3.141596;

struct GameSettings{
    int windowHeight;
    int windowWidth;
    int birdRadius = 32;
    int birdShiftLeft = 50;
    float gravity = 100.0;
    float birdHeightSpeedUp = 70.0;
    int pipeWidth = 100;
    float pipeSpeed = 100;
    float pipeBornDelay = 5000;
    int pipeSpace = 150;
    int pipeMinHeight = 50;
    GameSettings(){}
    GameSettings(int windowWidthIn, int windowHeightIn){
        windowHeight = windowHeightIn;
        windowWidth = windowWidthIn;
    }
};

class Pipe {
    QRect rect;
    //QPoint pos;
public:
    Pipe(){}
    Pipe(const QRect & rectIn/*, const QPoint & posIn*/){
        rect = rectIn;
        //pos = posIn;
    }
    void doStep(float deltaT, const GameSettings & gameSettings){
        //pos.setX(pos.x() - (int)(deltaT*gameSettings.pipeSpeed));
        //rect.setX(rect.x() - (int)(deltaT*gameSettings.pipeSpeed));
        rect.translate((int)(-deltaT*gameSettings.pipeSpeed),0);
    }
    QRect getRect() const{
        return rect;
    }
};

class Circle{
    QPoint pos;
    int rad;
public:
    Circle(){}
    Circle(const QPoint & posIn, const int radIn){
        pos = posIn;
        rad = radIn;
    }
    bool isIntersect(const QRect & rectIn){
        int leftX = rectIn.x();
        int upY = rectIn.y();
        int rightX = rectIn.x() + rectIn.width();
        int downY = rectIn.y() + rectIn.height();
        if (pos.x() <= leftX){
            if (pos.y() <= upY){
                if ((pos - QPoint(leftX,upY)).manhattanLength() <= rad){
                    return true;
                } else {
                    return false;
                }
            } else if (pos.y() < downY){
                if (leftX - pos.x() <= rad){
                    return true;
                } else {
                    return false;
                }
            } else {
                if ((pos - QPoint(leftX,downY)).manhattanLength() <= rad){
                    return true;
                } else {
                    return false;
                }
            }
        } else if (pos.x() < rightX){
            if (pos.y() <= upY){
                if (upY - pos.y() <= rad){
                    return true;
                } else {
                    return false;
                }
            } else if (pos.y() < downY){
                return true;
            } else {
                if (pos.y() - downY <= rad){
                    return true;
                } else {
                    return false;
                }
            }
        } else {
            if (pos.y() <= upY){
                if ((pos - QPoint(rightX,upY)).manhattanLength() <= rad){
                    return true;
                } else {
                    return false;
                }
            } else if (pos.y() < downY){
                if (pos.x() - rightX <= rad){
                    return true;
                } else {
                    return false;
                }
            } else {
                if ((pos - QPoint(rightX,downY)).manhattanLength() <= rad){
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    void setPos(const QPoint & posIn){
        pos = posIn;
    }
    QPoint & getPos(){
        return pos;
    }
};

class Bird {
    Circle circle;
    float heightSpeed;
    float birdGravity;
    float birdHeightSpeedUp;
public:
    Bird(){}
    Bird(const GameSettings & gameSettings){
        circle = Circle(QPoint(gameSettings.birdShiftLeft, gameSettings.windowHeight/2 - gameSettings.birdRadius), gameSettings.birdRadius);
        heightSpeed = 0;
        birdGravity = gameSettings.gravity;
        birdHeightSpeedUp = gameSettings.birdHeightSpeedUp;
    }
    void doStep(float deltaT, const GameSettings & gameSettings){
        QPoint & pos = circle.getPos();
        int y = (int)((float)(pos.y()) + heightSpeed*deltaT + (birdGravity*deltaT*deltaT)/2.0);
        if (y > gameSettings.windowHeight - gameSettings.birdRadius){
            y = gameSettings.windowHeight - gameSettings.birdRadius;
            heightSpeed = -heightSpeed*0.6;
        }
        else if (y < 0){
            y = 0;
            heightSpeed = -heightSpeed*0.6;
        }
        circle.setPos(QPoint(pos.x(), y));
        heightSpeed += birdGravity*deltaT;
    }
    void doFly(float deltaT, const GameSettings & gameSettings){
        heightSpeed -= birdHeightSpeedUp;
        doStep(deltaT, gameSettings);
    }
    QPoint getPosition(){
        return circle.getPos();
    }
    bool isIntersect(std::list<std::pair<Pipe, Pipe>> & pipes){
        auto iter = pipes.begin();
        if (iter != pipes.end()){
            if (circle.isIntersect(iter->first.getRect())){
                return true;
            } else if (circle.isIntersect(iter->second.getRect())){
                return true;
            }
            else {
                iter++;
                if (iter != pipes.end()){
                    if (circle.isIntersect(iter->first.getRect())){
                        return true;
                    } else if (circle.isIntersect(iter->second.getRect())){
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }
};


class Game {
    QDateTime lastTime;
    float pipeTime;
    GameSettings gameSettings;
    Bird bird;
    std::list<std::pair<Pipe, Pipe>> pipes;
    bool gameOver;
public:
    Game(){}
    Game(int windowHeightIn, int windowWidthIn) : gameSettings(windowHeightIn, windowWidthIn){
        bird = Bird(gameSettings);
        lastTime = QDateTime::currentDateTime();
        gameOver = false;
    }
    void doStep() {
        if (!gameOver){
            QDateTime newTime = QDateTime::currentDateTime();
            float deltaT = (newTime.toMSecsSinceEpoch() - lastTime.toMSecsSinceEpoch());
            lastTime = newTime;
            pipeTime += deltaT;
            if(pipeTime >= gameSettings.pipeBornDelay){
                pipeTime = pipeTime - gameSettings.pipeBornDelay;
                int pipeFirstDown = rand() % (gameSettings.windowHeight - 2*gameSettings.pipeMinHeight - gameSettings.pipeSpace+1);
                pipes.push_back(std::make_pair(Pipe(QRect(gameSettings.windowWidth - gameSettings.pipeWidth, 0, gameSettings.pipeWidth, gameSettings.pipeMinHeight+pipeFirstDown)),
                                            Pipe(QRect(gameSettings.windowWidth - gameSettings.pipeWidth, gameSettings.pipeMinHeight+pipeFirstDown+gameSettings.pipeSpace,
                                                       gameSettings.pipeWidth, gameSettings.windowHeight-(gameSettings.pipeMinHeight+pipeFirstDown+gameSettings.pipeSpace)))));
            }
            bird.doStep(deltaT/1000.0, gameSettings);
            while (pipes.size() > 0){
                if ((*pipes.begin()).first.getRect().x() + gameSettings.pipeWidth < 0){
                    pipes.pop_front();
                } else {
                    break;
                }
            }
            for (auto iter = pipes.begin(); iter != pipes.end(); iter++){
                iter->first.doStep(deltaT/1000.0, gameSettings);
                iter->second.doStep(deltaT/1000.0, gameSettings);
            }
            gameOver = bird.isIntersect(pipes);
        }
    }
    void spaceClicked() {
        if (!gameOver){
            QDateTime newTime = QDateTime::currentDateTime();
            float deltaT = (newTime.toMSecsSinceEpoch() - lastTime.toMSecsSinceEpoch())/1000.0;
            lastTime = newTime;
            bird.doFly(deltaT, gameSettings);
        }
    }
    QPoint getBirdPosition(){
        return QPoint(bird.getPosition().x() - gameSettings.birdRadius, bird.getPosition().y() - gameSettings.birdRadius);
    }
    std::list<std::pair<Pipe, Pipe>> & getPipes(){
        return pipes;
    }
    bool isGameOver(){
        gameOver = true;
    }
};

#endif // GAME_H
