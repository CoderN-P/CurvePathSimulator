#include "Waypoint.h"
#include "QuinticHermiteSpline.h"
#include <iostream>

Waypoint::Waypoint(double time, QuinticHermiteSpline parentSpline) {
    this->y = parentSpline.evaluatePoint(time, &QuinticHermiteSpline::basisFunctions, false);
    this->x = parentSpline.evaluatePoint(time, &QuinticHermiteSpline::basisFunctions, true);
    this->time = time;
    this->parentSpline = &parentSpline;
}

double Waypoint::getLinearVelocity(bool useX) const {
    return parentSpline->evaluateDerivative(time, 1, useX);
}

double Waypoint::getAngularVelocity() {
    double dx_dt = getLinearVelocity(true);
    double dy_dt = getLinearVelocity(false);
    double dy_dt_dt = parentSpline->evaluateDerivative(time, 2, false);
    double dx_dt_dt = parentSpline->evaluateDerivative(time, 2, true);

    return (dx_dt*dy_dt_dt - dy_dt*dx_dt_dt)/pow((pow(dx_dt, 2) + pow(dy_dt, 2)), 1.5);
}

double Waypoint::getTheta() {
    return atan2(getLinearVelocity(false), getLinearVelocity(true));
}


