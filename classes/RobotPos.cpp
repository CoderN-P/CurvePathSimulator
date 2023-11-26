#include "RobotPos.h"
#include <QPointF>

RobotPos::RobotPos(double x, double y, double theta, double velocity, double angularVelocity, double L) : QPointF(x, y) {
    this->theta = theta;
    this->velocity = velocity;
    this->angularVelocity = angularVelocity;
    this->L = L;
}

QPointF RobotPos::getXYVelocity() {
    double y = -L*angularVelocity*cos(theta)-velocity*sin(theta);
    double x = velocity/sin(theta) - y;
    return QPointF(x, y);
}