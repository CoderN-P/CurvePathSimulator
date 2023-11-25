//
// Created by GreatNeel on 11/18/23.
//

#include "QuinticHermiteSpline.h"
#include <QString>
#include "Waypoint.h"
#include <iostream>

double QuinticHermiteSpline::evaluatePoint(double p, const double (*customBasisFunctions)[6][6], bool x) const {
    double order[6];
    if (x){
        order[0] = start.x();
        order[1] = startVelocity.x();
        order[2] = startAcceleration.x();
        order[3] = endAcceleration.x();
        order[4] = endVelocity.x();
        order[5] = end.x();

    } else {
        order[0] = start.y();
        order[1] = startVelocity.y();
        order[2] = startAcceleration.y();
        order[3] = endAcceleration.y();
        order[4] = endVelocity.y();
        order[5] = end.y();
    }

    double result = 0;
    for (int i = 0; i < 6; i++){
        double temp = 0;
        for (int j = 0; j < 6; j++){
            temp += (*customBasisFunctions)[i][j] * pow(p, j);
        }
        result += temp*order[i];
    }

    return result;
}

double QuinticHermiteSpline::evaluateDerivative(double t, int order, bool x) {
    order = std::clamp(order, 0, 5);

    double customBasisFunctions[6][6];
    std::memcpy(customBasisFunctions, basisFunctions, sizeof(basisFunctions));

    for (int i = 0; i < order; i++){
        for (int j = 0; j < 6; j++){
            customBasisFunctions[i][j] *= j;
        }
    }

    return evaluatePoint(t, &customBasisFunctions, x);
}



QuinticHermiteSpline::QuinticHermiteSpline(QPointF start, QPointF end, QPointF startVelocity, QPointF endVelocity, QPointF startAcceleration, QPointF endAcceleration) {
    this->start = start;
    this->end = end;
    this->startVelocity = startVelocity;
    this->endVelocity = endVelocity;
    this->startAcceleration = startAcceleration;
    this->endAcceleration = endAcceleration;
    // Choose random color
    this->color = QColor(rand() % 256, rand() % 256, rand() % 256);
}



Waypoint QuinticHermiteSpline::getWaypoint(double time) {
    return {evaluatePoint(time), time, *this};
}
