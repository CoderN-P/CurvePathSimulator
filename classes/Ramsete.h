#pragma once
#include <QVector>
#include "RobotPos.h"
#include "Path.h"


class Ramsete {
    Path *path;
    double b;
    double zeta;

public:
    Ramsete(Path *path, double b, double zeta) : path(path), b(b), zeta(zeta) {}

    QVector<double> run(RobotPos *pos, double t);
};


