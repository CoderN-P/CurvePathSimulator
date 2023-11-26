#pragma once
#include <QPointF>
#include "RobotPos.h"
#include "ArduinoClient.h"

class PurePursuitPath;


class PurePursuit {
    double lookaheadDistance;
    double kdd;
    double minldd;
    double maxldd;
    double curSpeed;
    double wheelRadius;
    ArduinoClient *arduinoClient;
    PurePursuitPath *path;
    RobotPos curPos;
    double L;

public:
    PurePursuit(double kdd, double minldd, double maxldd, double l, double r, PurePursuitPath *path, ArduinoClient *arduinoClient);
    double getKdd() const;
    void setKdd(double kdd);
    double run();
};

