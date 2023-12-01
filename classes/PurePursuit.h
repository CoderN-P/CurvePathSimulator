#pragma once
#include <QPointF>
#include "RobotPos.h"
#include "ArduinoClient.h"

class PurePursuitPath;


class PurePursuit {
    double lookaheadDistance;
    double kdd;
    double curSpeed;
    double wheelRadius;
    ArduinoClient *arduinoClient;
    PurePursuitPath *path;
    RobotPos curPos;
    double L;

public:
    PurePursuit(double lookaheadDistance, double l, double r, PurePursuitPath *path, ArduinoClient *arduinoClient);

    QVector<double> run(RobotPos *pos = nullptr, int lastFoundIndex = 0);
};

