//
// Created by GreatNeel on 11/18/23.
//

#include "QuinticHermiteSpline.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Waypoint.h"
#include <iostream>

double QuinticHermiteSpline::evaluatePoint(double t, const double (*customBasisFunctions)[6][6]) {
    double order[6] = { start, startVelocity, startAcceleration, endAcceleration, endVelocity, end };
    double x = 0;
    /*
    for (int i = 0; i < 6; i++){
        double temp = 0;
        for (int j = 0; j < 6; j++){
            temp += (*customBasisFunctions)[i][j] * pow(t, j);
        }
        x += temp*order[i];
    }

    return x;
     */
    return t*t;

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

void QuinticHermiteSpline::draw(QGraphicsView *view, QGraphicsScene *scene, int points, int startPoint, int endPoint, double zoom, double pan) {
    int height = view->height();
    int width = view->width();
    int diff = endPoint - startPoint;
    double scaleX = (width)/(zoom*2*fabs(endPoint));
    QPoint prevPoint;
    for (int i = 0; i <= points; i++){
        double t = (diff*(double(i)/double(points))+startPoint);
        double y = evaluatePoint(t, &basisFunctions);

        y = height/2 - y*yScale;
        t = width/2 + t*(scaleX);

        if (i == 0){
            prevPoint = QPoint(t, y);
            continue;
        }

        QPoint point2 = QPoint(t, y);
        scene->addLine(prevPoint.x(), prevPoint.y(), point2.x(), point2.y());
        prevPoint = point2;
    }

}

QuinticHermiteSpline::QuinticHermiteSpline(double start, double end, double startVelocity, double endVelocity, double startAcceleration, double endAcceleration, double xScale, double yScale) {
    this->start = start;
    this->end = end;
    this->startVelocity = startVelocity;
    this->endVelocity = endVelocity;
    this->startAcceleration = startAcceleration;
    this->endAcceleration = endAcceleration;
    this->xScale = xScale;
    this->yScale = yScale;
}

Waypoint QuinticHermiteSpline::getWaypoint(double time) {
    return Waypoint(evaluatePoint(time), time, *this);
}