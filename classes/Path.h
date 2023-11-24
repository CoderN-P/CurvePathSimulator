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
    void draw(QGraphicsView *view, QGraphicsScene *scene, double zoom, int points, int ignore=-1,  bool changeScale=true, int ignoreIdx = -1, int ignoreIdx2 = -1);
    static void drawTicksAndLabels(double scaleX, double scaleY, int width, int height, QGraphicsScene *scene);
    static void drawGridLines(double scaleX, double scaleY, int width, int height, QGraphicsScene *scene);
    void drawControlPoints(int splineIdx, QColor *color, QPen *pen, int ignore);
    void drawStartControlPoints(QColor *color, QPen *pen, int ignore, bool start=true);
    void drawMiddleControlPoints(int splineIdx, QColor *color, QPen *pen, int ignore);
};



