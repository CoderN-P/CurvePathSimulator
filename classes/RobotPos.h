#pragma once
#include <QPointF>


class RobotPos: public QPointF {
public:
    double theta;
    double velocity;
    double angularVelocity;
    double L;
    RobotPos(){
        theta = 0;
        velocity = 0;
        angularVelocity = 0;
        L = 0;
    };
    RobotPos(double x, double y, double theta, double velocity, double angularVelocity, double L);
    QPointF getXYVelocity();

};

