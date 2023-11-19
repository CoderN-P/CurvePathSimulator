//
// Created by GreatNeel on 11/18/23.
//

#include "Waypoint.h"
#include "QuinticHermiteSpline.h"

Waypoint::Waypoint(double y, double time, QuinticHermiteSpline parentSpline) {
    this->y = y;
    this->time = time;
    this->parentSpline = &parentSpline;
}

double Waypoint::getLinearVelocity() {
    return (*parentSpline).evaluateDerivative(time, 1);
}

double Waypoint::getAngularVelocity() {
    double linearVelocity = getLinearVelocity();
    double curvature = (*parentSpline).evaluateDerivative(time, 2);
    double radius = 1/curvature;
    return linearVelocity/radius;
}
