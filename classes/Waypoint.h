//
// Created by GreatNeel on 11/18/23.
//

#ifndef CURVEPATHSIMULATOR_WAYPOINT_H
#define CURVEPATHSIMULATOR_WAYPOINT_H

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


#endif //CURVEPATHSIMULATOR_WAYPOINT_H
