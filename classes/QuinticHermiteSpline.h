#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>

class Waypoint;

class QuinticHermiteSpline {
    double startVelocity, endVelocity, startAcceleration, endAcceleration;

public:
    QuinticHermiteSpline(){
        return;
    }
    QuinticHermiteSpline(double start, double end, double startVelocity, double endVelocity, double startAcceleration, double endAcceleration);
    double evaluateDerivative(double t, int order);
    double evaluatePoint(double t, const double (*customBasisFunctions)[6][6] = &basisFunctions);
    Waypoint getWaypoint(double time);
    double start;
    double end;
/*
 https://www.rose-hulman.edu/~finn/CCLI/Notes/day09.pdf
 */
constexpr static const double basisFunctions[6][6] = {
        {1, 0, 0, -10, 15, -6},
        {0, 1, 0, -6, 8, -4},
        {0, 0, 0.5, -1.5, 1.5, -0.5},
        {0, 0, 0, 0.5, -1, 0.5},
        {1, 0, 0, -4, 7, -3},
        {0, 0, 0, 10, -15, 6},
};
};

