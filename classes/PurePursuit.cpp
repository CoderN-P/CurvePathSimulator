#include "PurePursuitPath.h"
#include "Path.h"
#include "PurePursuit.h"
#include "RobotPos.h"
#include "ArduinoClient.h"
#include <iostream>

PurePursuit::PurePursuit(double lookaheadDistance, double l, double R, PurePursuitPath *path, ArduinoClient *arduinoClient) {
    this->lookaheadDistance = lookaheadDistance;
    this->curSpeed = 0;
    this->path = path;
    this->wheelRadius = R;
    Waypoint start = path->path->splines[0].getWaypoint(0);
    // Assume the robot starts at the beginning of the path
    this->L = l;
    this->arduinoClient = arduinoClient;

}

double findMinAngle(double absTargetAngle, double curAngle) {
    double diff = absTargetAngle - curAngle;

    if (diff > 180) {
        diff -= 360;
    } else if (diff < -180) {
        diff += 360;
    }

    return diff;
}


QVector<double> PurePursuit::run(RobotPos *pos, int lastFoundIndex) {
    if (!pos->isNull()) {
        curPos = *pos;
    } else {
        curPos = arduinoClient->getPosition();
    }

    // Step 1. Find the goal point
    QVector<double> data = path->findGoalPoint(QPointF(curPos.x(), curPos.y()), lookaheadDistance, lastFoundIndex);

    QPointF goalPoint = QPointF(data[0], data[1]);
    lastFoundIndex = data[2];

    double Kp = 3;
    double absTargetAngle = atan2(goalPoint.y() - curPos.y(), goalPoint.x() - curPos.x())*180/M_PI;
    double curAngle = curPos.theta;
    double diff = absTargetAngle - curAngle;

    if (diff > 180) {
        diff -= 360;
    } else if (diff < -180) {
        diff += 360;
    }

    double turn = Kp*diff;

    return QVector<double>({goalPoint.x(), goalPoint.y(), turn, double(lastFoundIndex)});
}


