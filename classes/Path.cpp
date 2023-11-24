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



void Path::draw(QGraphicsView *view, QGraphicsScene *scene, double zoom, int points, bool controlPoints, bool changeScale){

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
                if (i == 0 && controlPoints){
                    drawControlPoints(idx, &colors[idx], &curvePen);
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

void Path::drawControlPoints(int splineIdx, QColor *color, QPen *curvePen) {
    // Draw a circle to mark the start of a spline
    QGraphicsScene &scene = *parent->graphicsScene_;
    double x = splines[splineIdx].start.x();
    double y = splines[splineIdx].start.y();
    double width = scene.width();
    double height = scene.height();
    double ystart = height / 2 - y * scaleY;
    double xstart = width / 2 + x * scaleX;
    auto *circle = new ControlPointUI(0, splineIdx, this);
    QBrush brush(Qt::SolidPattern);
    brush.setColor(*color);
    circle->setRect(0, 0, 10, 10);
    circle->setPos(xstart-5, ystart-5);

    circle->setBrush(brush);
    circle->setPen(*curvePen);
    circle->setZValue(1000);
    circle->setOpacity(0.9);
    circle->setParent(&scene);
    scene.addItem(circle);


    // Draw a circle to mark the velocity tangent

    double xV = splines[splineIdx].startVelocity.x();
    double yV = splines[splineIdx].startVelocity.y();
    xV = width / 2 + (x + xV) * scaleX;
    yV = height / 2 - (y + yV) * scaleY;
    auto *circleV = new ControlPointUI(2, splineIdx, this);
    circleV->setRect(0, 0, 10, 10);
    circleV->setPos(xV-5, yV-5);
    circleV->setBrush(brush);
    circleV->setPen(*curvePen);
    circleV->setZValue(1000);
    circleV->setOpacity(0.9);
    circleV->setParent(&scene);
    scene.addItem(circleV);

    QLineF line = QLineF(QPointF(xstart, ystart), QPointF(xV, yV));
    // Make line dotted and gray
    QPen *curvePen2 = new QPen();
    curvePen2->setColor(Qt::gray);
    curvePen2->setStyle(Qt::DashLine);
    curvePen2->setWidthF(2.0);
    // Label the line
    QGraphicsTextItem *text = scene.addText("Velocity");
    text->setPos((line.p1().x() + line.p2().x() - text->boundingRect().width()) / 2,
                 line.p2().y() + 5);
    scene.addLine(line, *curvePen2);

    // Draw a circle to mark the acceleration tangent

    double xA = splines[splineIdx].startAcceleration.x();
    double yA = splines[splineIdx].startAcceleration.y();
    xA = width / 2 + (x + xA) * scaleX;
    yA = height / 2 - (y + yA) * scaleY;
    auto *circleA = new ControlPointUI(3, splineIdx, this);
    circleA->setRect(0, 0, 10, 10);
    circleA->setPos(xA-5, yA-5);
    circleA->setBrush(brush);
    circleA->setPen(*curvePen);
    circleA->setZValue(1000);
    circleA->setOpacity(0.9);
    circleA->setParent(&scene);
    scene.addItem(circleA);

    line = QLineF(QPointF(xstart, ystart), QPointF(xA, yA));

    text = scene.addText("Acceleration");
    text->setPos((line.p1().x() + line.p2().x() - text->boundingRect().width()) / 2,
                 line.p2().y() + 5);
    scene.addLine(line, *curvePen2);
}
