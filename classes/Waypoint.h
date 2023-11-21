#pragma once

class QuinticHermiteSpline;

class Waypoint {
    double x, y;
    double time;
    QuinticHermiteSpline* parentSpline;
public:
    Waypoint(double y,  double time, QuinticHermiteSpline parentSpline);
    double getAngularVelocity();
    double getLinearVelocity();
};

