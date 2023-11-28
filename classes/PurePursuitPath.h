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
    Waypoint getClosestPoint(QPointF point);
    Waypoint getNextPoint(Waypoint point);
};
