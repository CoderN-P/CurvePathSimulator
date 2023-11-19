//
// Created by GreatNeel on 11/18/23.
//

#include "QuinticHermiteSpline.h"
#include "Waypoint.h"

double QuinticHermiteSpline::evaluatePoint(double t, const double (*customBasisFunctions)[6][6]) {
    double order[6] = { start, startVelocity, startAcceleration, endAcceleration, endVelocity, end };
    double x = 0;
    for (int i = 0; i < 6; i++){
        double temp = 0;
        for (int j = 0; j < 6; j++){
            temp += (*customBasisFunctions)[i][j] * pow(t, j);
        }
        x += temp*order[i];
    }
    return x;
}

double QuinticHermiteSpline::evaluateDerivative(double t, int order) {
    order = std::clamp(order, 0, 5);

    double customBasisFunctions[6][6];
    std::memcpy(customBasisFunctions, basisFunctions, sizeof(basisFunctions));

    for (int i = 0; i < order; i++){
        for (int j = 0; j < 6; j++){
            customBasisFunctions[i][j] *= j;
        }
    }

    return evaluatePoint(t, &customBasisFunctions);
}

void QuinticHermiteSpline::draw(QPainter *painter, int points) {
    QPoint prevPoint = QPoint(0, start*scale);
    for (int i = 0; i < points; i++){
        QPoint point2 = QPoint((i+1)*scale/points, evaluatePoint((i+1)*scale/points, &basisFunctions)*scale);
        painter->drawLine(prevPoint, point2);
        prevPoint = point2;
    }

}

QuinticHermiteSpline::QuinticHermiteSpline(double start, double end, double startVelocity, double endVelocity, double startAcceleration, double endAcceleration, double scale) {
    this->start = start;
    this->end = end;
    this->startVelocity = startVelocity;
    this->endVelocity = endVelocity;
    this->startAcceleration = startAcceleration;
    this->endAcceleration = endAcceleration;
    this->scale = scale;
}

Waypoint QuinticHermiteSpline::getWaypoint(double time) {
    return Waypoint(evaluatePoint(time), time, *this);
}