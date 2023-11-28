#include <QPointF>
#include "QuinticHermiteSpline.h"
#include "Path.h"
#include "PurePursuitPath.h"


PurePursuitPath::PurePursuitPath(Path *path) : path(path) {}

void PurePursuitPath::generateWaypoints(double spacing) {
    double distance = 0;
    for (int i = 0; i < path->splines.size(); i++) {
        QuinticHermiteSpline spline = path->splines[i];
        for (double t = 0; t < 1; t += spacing) {
            Waypoint point = spline.getWaypoint(t);
            waypoints.append(point);
        }
    }
}

Waypoint PurePursuitPath::getClosestPoint(QPointF point) {
    Waypoint closestPoint = waypoints[0];
    double closestDistance = sqrt(pow(point.x() - closestPoint.x, 2) + pow(point.y() - closestPoint.y, 2));
    for (int i = 1; i < waypoints.size(); i++) {
        Waypoint curPoint = waypoints[i];
        double distance = sqrt(pow(point.x() - curPoint.x, 2) + pow(point.y() - curPoint.y, 2));
        if (distance < closestDistance) {
            closestDistance = distance;
            closestPoint = curPoint;
        }
    }
    return closestPoint;
}

Waypoint PurePursuitPath::getNextPoint(Waypoint point) {
    int index = waypoints.indexOf(point);
    if (index == waypoints.size() - 1) {
        return waypoints[index];
    }
    return waypoints[index + 1];
}