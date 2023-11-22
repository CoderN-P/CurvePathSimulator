#pragma once
#include <QVector>
#include <QGraphicsView>
#include <QGraphicsView>
#include "QuinticHermiteSpline.h"

class Path {

public:
    QVector<QuinticHermiteSpline> splines;
    explicit Path(QVector<QuinticHermiteSpline> splines);
    void draw(QGraphicsView *view, QGraphicsScene *scene, double zoom, int points);
    static void drawTicksAndLabels(double scaleX, double scaleY, int width, int height, QGraphicsScene *scene);
    static void drawGridLines(double scaleX, double scaleY, int width, int height, QGraphicsScene *scene);
};



