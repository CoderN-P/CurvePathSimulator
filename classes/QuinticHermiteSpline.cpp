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
            if (p == 0 && j == 0){
                temp += (*customBasisFunctions)[i][j];
            } else {
                temp += (*customBasisFunctions)[i][j] * pow(p, j);
            }
        }

        result += temp*order[i];
    }

    return result;
}

double QuinticHermiteSpline::evaluateDerivative(double t, int ord, bool x) {
    ord = std::clamp(ord, 0, 5);

    double customBasisFunctions[6][6];
    // Fill customBasisFunctions with the correct values
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            customBasisFunctions[i][j] = basisFunctions[i][j];
        }
    }

    for (int o = 0; o < ord; o++){
        for (int i = 0; i < 6; i++){
            for (int j = 0; j < 5-o; j++){
                customBasisFunctions[i][j] = customBasisFunctions[i][j+1]*(j+1);
            }
            customBasisFunctions[i][5-o] = 0;
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

double QuinticHermiteSpline::evaluateCurvature(double t) {
    double xp = evaluateDerivative(t, 1, true);
    double xpp = evaluateDerivative(t, 2, true);
    double yp = evaluateDerivative(t, 1, false);
    double ypp = evaluateDerivative(t, 2, false);

    double numerator = std::abs(xp*ypp - yp*xpp);
    double denominator = pow(pow(xp, 2) + pow(yp, 2), 1.5);

    if (denominator == 0) {
        return 0;
    }

    return numerator/denominator;

}

Waypoint QuinticHermiteSpline::getWaypoint(double time) {
    return {time, *this};
}
