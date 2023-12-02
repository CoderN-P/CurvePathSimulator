#include <QPointF>
#include "QuinticHermiteSpline.h"
#include "Path.h"
#include "PurePursuitPath.h"
#include <iostream>


PurePursuitPath::PurePursuitPath(Path *path) : path(path) {}

void PurePursuitPath::generateWaypoints(double spacing) {
    int i = 0;
    for (QuinticHermiteSpline& curSpline : path->splines) {
        if (i == 0) {
            waypoints.append(curSpline.getWaypoint(0));
        }
        double t = 0;
        while (t < 1) {
            double dist = 0;

            double x1 = curSpline.evaluatePoint(t, &QuinticHermiteSpline::basisFunctions, true);
            double y1 = curSpline.evaluatePoint(t, &QuinticHermiteSpline::basisFunctions, false);

            while (dist < spacing) {
                double step = 0.001;
                double curvature = curSpline.evaluateCurvature(t);
                if (curvature != 0) {
                    step = 0.001/curvature;
                }
                double x2 = curSpline.evaluatePoint(t + step, &QuinticHermiteSpline::basisFunctions, true);
                double y2 = curSpline.evaluatePoint(t + step, &QuinticHermiteSpline::basisFunctions, false);
                double dx = x2 - x1;
                double dy = y2 - y1;
                dist += pow(pow(dx, 2) + pow(dy, 2), 0.5);
                t += step;
                x1 = x2;
                y1 = y2;
            }
            if (t > 1) {
                t = 1;
            }
            waypoints.append(curSpline.getWaypoint(t));
        }
        i++;
    }

    // Draw the waypoints

    int j = 0;
    for (Waypoint waypoint : waypoints) {
        j++;
        QGraphicsScene *scene = path->parent->graphicsScene_;
        double scaleX = path->scaleX;
        double scaleY = path->scaleY;
        double width = path->parent->graphicsView_->width();
        double height = path->parent->graphicsView_->height();
        double x = waypoint.x*scaleX + width/2;
        double y = -waypoint.y*scaleY + height/2;
        scene->addEllipse(x-2, y-2, 4, 4, QPen(Qt::black), QBrush(Qt::black));
    }
}

int sign(double x) {
    if (x < 0) {
        return -1;
    }
    return 1;
}

QVector<QPointF> PurePursuitPath::findIntersectPoints(QPointF curPoint, QPointF p1, QPointF p2, double lookaheadDistance) {
    p2.setX(p2.x()-curPoint.x());
    p2.setY(p2.y()-curPoint.y());
    p1.setX(p1.x()-curPoint.x());
    p1.setY(p1.y()-curPoint.y());

    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();

    double a = dx*dx + dy*dy;
    double dr = pow(a, 0.5);
    double D = p1.x()*p2.y() - p2.x()*p1.y();
    double discriminant = pow(lookaheadDistance, 2)*pow(dr, 2) - pow(D, 2);

    if (discriminant < 0) {
        return {};
    }

    double x1 = (D*dy + sign(dy)*dx*pow(discriminant, 0.5))/pow(dr, 2);
    double x2 = (D*dy - sign(dy)*dx*pow(discriminant, 0.5))/pow(dr, 2);
    double y1 = (-D*dx + abs(dy)*pow(discriminant, 0.5))/pow(dr, 2);
    double y2 = (-D*dx - abs(dy)*pow(discriminant, 0.5))/pow(dr, 2);

    QPointF sol1 = QPointF(x1+curPoint.x(), y1+curPoint.y());
    QPointF sol2 = QPointF(x2+curPoint.x(), y2+curPoint.y());

    return QVector<QPointF>{sol1, sol2};


}

double distance(QPointF p1, QPointF p2) {
    return pow(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2), 0.5);
}


QVector<double> PurePursuitPath::findGoalPoint(QPointF curPos, double lookaheadDistance, int lastFoundIndex) {
    // Step 1. Find the goal point

    QPointF goalPoint;
    bool intersectFound = false;
    int curIndex = lastFoundIndex;

    for (int i = lastFoundIndex; i < waypoints.size()-1; i++) {
        Waypoint waypoint = waypoints[i];
        Waypoint nextWaypoint = waypoints[i+1];
        QVector<QPointF> circleIntersect = findIntersectPoints(curPos, QPointF(waypoint.x, waypoint.y), QPointF(nextWaypoint.x, nextWaypoint.y), lookaheadDistance);
        if (circleIntersect.empty()) {
            continue;
        }
        if (circleIntersect[0].isNull() && circleIntersect[1].isNull()) {
            continue;
        }

        double minX = std::min(waypoint.x, nextWaypoint.x);
        double maxX = std::max(waypoint.x, nextWaypoint.x);
        double minY = std::min(waypoint.y, nextWaypoint.y);
        double maxY = std::max(waypoint.y, nextWaypoint.y);

        if ((circleIntersect[0].x() >= minX && circleIntersect[0].x() <= maxX) || (circleIntersect[0].y() >= minY && circleIntersect[0].y() <= maxY)) {
            intersectFound = true;
            if ((circleIntersect[0].x() >= minX && circleIntersect[0].x() <= maxX) && (circleIntersect[0].y() >= minY && circleIntersect[0].y() <= maxY)){
                // Both x and y are in the range
                // Choose the one that is closer to the next waypoint
                if (distance(circleIntersect[0], QPointF(nextWaypoint.x, nextWaypoint.y)) < distance(circleIntersect[1], QPointF(nextWaypoint.x, nextWaypoint.y))) {
                    goalPoint = circleIntersect[0];
                } else {
                    goalPoint = circleIntersect[1];
                }

            } else {
                // Only one of x and y is in the range
                if (circleIntersect[0].x() >= minX && circleIntersect[0].x() <= maxX) {
                    goalPoint = circleIntersect[0];
                } else {
                    goalPoint = circleIntersect[1];
                }
            }

            if (distance(goalPoint, QPointF(nextWaypoint.x, nextWaypoint.y)) < distance(curPos, QPointF(nextWaypoint.x, nextWaypoint.y))) {
                curIndex = i;
                break;
            } else {
                curIndex = i+1;
            }
        } else {
            intersectFound = false;
            goalPoint = QPointF(waypoints[curIndex].x, waypoints[curIndex].y);
        }

    }


    return {goalPoint.x(), goalPoint.y(), double(curIndex)};
}
