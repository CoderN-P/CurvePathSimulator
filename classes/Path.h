#pragma once
#include <QVector>
#include <QGraphicsView>
#include <QGraphicsView>
#include "QuinticHermiteSpline.h"
#include "../ui/mainwindow.h"

class Path {

public:
    QVector<QuinticHermiteSpline> splines;
    double scaleX;
    double scaleY;
    MainWindow *parent;
    explicit Path(QVector<QuinticHermiteSpline> splines, MainWindow *parent = nullptr);
    void draw(QGraphicsView *view, QGraphicsScene *scene, double zoom, int points, bool drawControlPoints=true, bool changeScale=true);
    static void drawTicksAndLabels(double scaleX, double scaleY, int width, int height, QGraphicsScene *scene);
    static void drawGridLines(double scaleX, double scaleY, int width, int height, QGraphicsScene *scene);
};



