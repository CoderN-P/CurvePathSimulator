#pragma once
#include "Waypoint.h"
#include <QVector>
#include <QPointF>

class Path;

/*
 * PurePursuitPath is a class that represents a path that can be used by PurePursuit.
 * It defines specific waypoints to break up the math into multiple points
 */


class PurePursuitPath {

public:
    Path *path;
    QVector<Waypoint> waypoints;
    PurePursuitPath(Path *path);
    void generateWaypoints(double spacing);
    static QVector<QPointF> findIntersectPoints(QPointF curPoint, QPointF p1, QPointF p2, double lookaheadDistance);
    QVector<double> findGoalPoint(QPointF curPos, double lookaheadDistance, int lastFoundIndex);
};
