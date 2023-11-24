#include <QGraphicsTextItem>
#include <QEvent>
#include <QSlider>
#include <QGraphicsSceneMouseEvent>
#include "Path.h"
#include "ControlPointUI.h"
#include <iostream>
#include <utility>


Path::Path(QVector<QuinticHermiteSpline> splineList, MainWindow *parent) {
    this->splines = std::move(splineList);
    this->parent = parent;
}


void Path::drawGridLines(double scaleX, double scaleY, int width, int height, QGraphicsScene *scene) {
    for (int i = 0; i < 5*width/scaleX; i++){
        scene->addLine(i*scaleX/5, 0, i*scaleX/5, height, QPen(Qt::lightGray));
    }

    for (int i = 0; i < 5*height/scaleY; i++){
        scene->addLine(0, i*scaleY/5, width, i*scaleY/5, QPen(Qt::lightGray));
    }
}

void Path::drawTicksAndLabels(double scaleX, double scaleY, int width, int height, QGraphicsScene *scene){
    int xAmount = width / scaleX;
    int yAmount = height / scaleY;

    int xDigits = int(log10(xAmount/2));
    // Check if xAmount is a power of 10 greater than 1
    if (xAmount / 2 == pow(10, xDigits) && xDigits > 0) {
        xDigits--;
    }

    for (int i = 1; i <= xAmount; i++) {
        if (i == xAmount / 2 || i % int(pow(10, xDigits)) != 0) {
            continue;
        }
        if (i < xAmount / 2) {
            scene->addLine(width / 2 - i * scaleX, height / 2 - 5, width / 2 - i * scaleX, height / 2 + 5);
            QGraphicsTextItem *text = scene->addText(QString::number(-i));
            text->setPos(width / 2 - i * scaleX, height / 2 + 10);
        } else {
            scene->addLine(i * scaleX, height / 2 - 5, i * scaleX, height / 2 + 5);
            QGraphicsTextItem *text = scene->addText(QString::number(i-xAmount/2));
            text->setPos(i * scaleX, height / 2 + 10);
        }
    }
}



void Path::draw(QGraphicsView *view, QGraphicsScene *scene, double zoom, int points, bool drawControlPoints, bool changeScale){

    int height = view->height();
    int width = view->width();

    if (splines.isEmpty()){
        return;
    }

    if (changeScale) {
        double maxX = 0;
        for (QuinticHermiteSpline spline: splines) {
            maxX = std::max(maxX, std::max(fabs(spline.start.x()), fabs(spline.end.x())));
        }

        // Check if startPoint or endPoint is greater in magnitude
        // The one with the greater magnitude will be used to determine how many x ticks are needed
        scaleX = (width) / (zoom * 2 * fabs(maxX));


        scaleY = scaleX * (double(height) / width);
    }



    // Draw ticks and labels
    drawGridLines(scaleX, scaleY, width, height, scene);
    drawTicksAndLabels(scaleX, scaleY, width, height, scene);
    parent->drawAxis();
    auto *group = new QGraphicsItemGroup();
    QPointF prevPoint;

    QPen curvePen;
    curvePen.setWidthF(2.0);       // Set the line width
    curvePen.setStyle(Qt::SolidLine);  // Set the line style

    QColor colors[3] = {Qt::blue, Qt::red, Qt::green};

    for (int idx = 0; idx < splines.size(); idx++) {
        QuinticHermiteSpline spline = splines[idx];
        curvePen.setColor(colors[idx]);
        for (int i = 0; i <= points; i++) {
            double t = (double(i) / double(points));
            double x = spline.evaluatePoint(t, &QuinticHermiteSpline::basisFunctions, true);
            double y = spline.evaluatePoint(t, &QuinticHermiteSpline::basisFunctions, false);

            y = double(height) / 2 - y * scaleY;

            x = double(width) / 2 + x * (scaleX);


            if (i == 0 || y < 0 || y > height){
                prevPoint = QPointF(x, y);
                if (i == 0 && drawControlPoints){
                    // Draw a circle to mark the start of a spline
                    auto *circle = new ControlPointUI(0, idx, this);
                    // fill circle
                    QBrush brush(Qt::SolidPattern);
                    circle->setRect(0, 0, 10, 10);
                    circle->setPos(x-5, y-5);
                    brush.setColor(colors[idx]);
                    circle->setBrush(brush);
                    circle->setPen(curvePen);
                    // set circle to highest z value
                    circle->setZValue(1000);
                    // lower opacity by 10%
                    circle->setOpacity(0.9);
                    circle->setParent(scene);
                    scene->addItem(circle);

                }
                continue;
            }
            QPointF point = QPointF(x, y);
            auto *curve = new QGraphicsLineItem(QLineF(prevPoint,point));
            // get global position of curve

            curve->setPen(curvePen);
            group->addToGroup(curve);
            prevPoint = point;
        }
    }

    scene->addItem(group);
}
