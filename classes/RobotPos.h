#pragma once
#include <QPointF>


class RobotPos: public QPointF {
public:
    double theta;
    double velocity;
    double angularVelocity;
    double L;
    RobotPos(double x, double y, double theta, double velocity, double angularVelocity, double L);
    QPointF getXYVelocity();

};

