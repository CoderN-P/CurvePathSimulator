#pragma once
#include <QGraphicsScene>
#include <QPoint>
#include <QGraphicsView>

class Waypoint;

class QuinticHermiteSpline {

public:
    QuinticHermiteSpline(){
        return;
    }
    QuinticHermiteSpline(QPoint start, QPoint end, QPoint startVelocity, QPoint endVelocity, QPoint startAcceleration, QPoint endAcceleration);
    double evaluateDerivative(double t, int order, bool x);
    double evaluatePoint(double p, const double (*customBasisFunctions)[6][6] = &basisFunctions, bool x = true) const;
    Waypoint getWaypoint(double time);
    QPoint start;
    QPoint end;
/*
 https://www.rose-hulman.edu/~finn/CCLI/Notes/day09.pdf
 */
constexpr static const double basisFunctions[6][6] = {
        {1, 0, 0, -10, 15, -6},
        {0, 1, 0, -6, 8, -3},
        {0, 0, 0.5, -1.5, 1.5, -0.5},
        {0, 0, 0, 0.5, -1, 0.5},
        {0, 0, 0, -4, 7, -3},
        {0, 0, 0, 10, -15, 6},
};
    QPoint startAcceleration;
    QPoint startVelocity;
    QPoint endAcceleration;
    QPoint endVelocity;
};

