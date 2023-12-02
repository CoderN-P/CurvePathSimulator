#include "Ramsete.h"
#include "Waypoint.h"
#include <iostream>

QVector<double> Ramsete::run(RobotPos *pos, double t){

    int splineIdx = floor(t/2000);
    if (fmod(t, 2000) == 0 && t != 0){
        splineIdx -= 1;
    }


    Waypoint curWaypoint = Waypoint((t - splineIdx*2000)/2000, path->splines[splineIdx]);


    double xGoal = curWaypoint.x;
    double yGoal = curWaypoint.y;
    double thetaGoal = curWaypoint.getTheta();



    pos->theta = pos->theta*M_PI/180;

    double xError = xGoal - pos->x();
    double yError = yGoal - pos->y();
    double thetaError = thetaGoal - pos->theta;

    xError = cos(pos->theta)*(xError) + sin(pos->theta)*(yError);
    yError = -sin(pos->theta)*(xError) + cos(pos->theta)*(yError);

    double linVelGoal = sqrt(pow(curWaypoint.getLinearVelocity(true), 2) + pow(curWaypoint.getLinearVelocity(false), 2));
    double angVelGoal = curWaypoint.getAngularVelocity();



    double k = 2*zeta*sqrt(pow(angVelGoal, 2) + b*pow(linVelGoal, 2));
    double v = linVelGoal*cos(thetaError) + k*xError;
    double w = angVelGoal + k*thetaError + b*linVelGoal*yError;

    std::cout << "v: " << linVelGoal << " w: " << angVelGoal << std::endl;

    return QVector<double>({v, w});

}