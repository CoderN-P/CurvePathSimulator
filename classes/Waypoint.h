#pragma once

class QuinticHermiteSpline;

class Waypoint {

public:
    double x, y;
    double time;
    QuinticHermiteSpline* parentSpline;
    Waypoint() {
        return;
    }
    Waypoint(double time, QuinticHermiteSpline parentSpline);
    double getAngularVelocity();
    double getTheta();
    double getLinearVelocity(bool useX = true) const;
    bool operator==(const Waypoint& other) const {
        return time == other.time && x == other.x && y == other.y && parentSpline == other.parentSpline;
    }
};

