#include "PurePursuitPath.h"
#include "Path.h"
#include "PurePursuit.h"
#include "RobotPos.h"
#include "ArduinoClient.h"

PurePursuit::PurePursuit(double kdd, double minldd, double maxldd, double l, double R, PurePursuitPath *path, ArduinoClient *arduinoClient) {
    this->kdd = kdd;
    this->minldd = minldd;
    this->maxldd = maxldd;
    this->curSpeed = 0;
    this->path = path;
    this->wheelRadius = R;
    Waypoint start = path->path->splines[0].getWaypoint(0);
    // Assume the robot starts at the beginning of the path
    this->L = l;
    this->arduinoClient = arduinoClient;

}

double PurePursuit::getKdd() const {
    return kdd;
}

void PurePursuit::setKdd(double kdd) {
    PurePursuit::kdd = kdd;
}

double PurePursuit::run() {
    // Step 1. Compute lookahead distance
    curPos = arduinoClient->getPosition();

    if (curPos.velocity * kdd < minldd) {
        lookaheadDistance = minldd;
    } else if (curSpeed * kdd > maxldd) {
        lookaheadDistance = maxldd;
    } else {
        lookaheadDistance = curPos.velocity * kdd;
    }


    // Step 2. Find the point on the path that is closest to the lookahead distance
    Waypoint closestPoint = path->getClosestPoint(QPointF(curPos.x(), curPos.y()));
    Waypoint curPoint = closestPoint;

    while (true) {
        double distance = sqrt(pow(curPoint.x - closestPoint.x, 2) + pow(curPoint.y - closestPoint.y, 2));
        if (distance > lookaheadDistance) {
            break;
        }
        Waypoint temp = path->getNextPoint(curPoint);
        if (temp == curPoint) {
            break;
        }
        curPoint = temp;
    }

    // curPoint is now the target point

    double alpha = atan2(curPoint.y - curPos.y(), curPoint.x - curPos.x());

    // delta is the angle between the robot's heading and the line between the robot and the target point
    double delta = atan2(2 * L * sin(alpha), lookaheadDistance);

    // Step 3. Calculate required change in linear and angular velocity
    double xV = curPoint.getLinearVelocity();
    double yV = curPoint.getLinearVelocity(false);

    QPointF robotXYVel = curPos.getXYVelocity();
    double xVelChange = xV - robotXYVel.x();
    double yVelChange = yV - robotXYVel.y();
    double velocityChange = cos(delta) * xVelChange + sin(delta) * yVelChange;
    double angularVelocityChange = (sin(delta) * xVelChange - cos(delta) * yVelChange) / L;

    double vChangeLeft = 2*velocityChange - angularVelocityChange*L;
    double vChangeRight = 2*velocityChange + angularVelocityChange*L;

    // TODO: Send vChangeLeft and vChangeRight to the arduino

    arduinoClient->setVelocity(vChangeLeft, vChangeRight);
    return delta;
}


