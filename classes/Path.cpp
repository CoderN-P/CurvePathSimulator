#include <QGraphicsTextItem>
#include <QEvent>
#include <QSlider>
#include <QGraphicsSceneMouseEvent>
#include "Path.h"
#include "ControlPointUI.h"
#include <iostream>
#include <utility>
#include <QPropertyAnimation>
#include <QTimeLine>
#include "AnimatedObject.h"

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

    if (fmod(xAmount, 2) == 1){
        xAmount -= 1;
        // w/x - 1 = w/z
        // w = z(w/x - 1)
        // z = w/(w/x - 1)
        scaleX = width/(width/scaleX - 1);
        scaleY = scaleX;
    }

    if (fmod(yAmount, 2) == 1){
        yAmount -= 1;
        scaleY = height/(height/scaleY - 1);
        scaleX = scaleY;
    }


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

    int yDigits = int(log10(yAmount/2));
    // Check if yAmount is a power of 10 greater than 1
    if (yAmount / 2 == pow(10, yDigits) && yDigits > 0) {
        yDigits--;
    }




    for (int i = 1; i <= yAmount; i++) {
        if (i == int(yAmount / 2) || i % int(pow(10, yDigits)) != 0) {
            continue;
        }
        if (i < yAmount / 2) {
            scene->addLine(width / 2 - 5, height / 2 - i * scaleY, width / 2 + 5, height / 2 - i * scaleY);
            QGraphicsTextItem *text = scene->addText(QString::number(i));
            text->setPos(width / 2 - 20, height / 2 - i * scaleY);
        } else {
            scene->addLine(width / 2 - 5, i * scaleY, width / 2 + 5, i * scaleY);
            QGraphicsTextItem *text = scene->addText(QString::number(-i+yAmount/2));
            text->setPos(width / 2 - 20, i * scaleY);
        }
    }
}



void Path::draw(QGraphicsView *view, QGraphicsScene *scene, double zoom, int points, int ignore, bool changeScale, int ignoreIdx, int ignoreIdx2){
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
        scaleY = scaleX;
        drawGridLines(scaleX, scaleY, width, height, scene);
        drawTicksAndLabels(scaleX, scaleY, width, height, scene);
        parent->drawAxis();
    }

    // Draw ticks and labels

    QPointF prevPoint;

    QPen curvePen;
    curvePen.setWidthF(4.0);       // Set the line width
    curvePen.setStyle(Qt::SolidLine);

    for (int idx = 0; idx < splines.size(); idx++) {
        if (idx != ignoreIdx && ignoreIdx != -1){
            if (ignoreIdx2 == -1) {
                continue;
            }
        }
        if (idx != ignoreIdx2 && ignoreIdx2 != -1){
            if (ignoreIdx == -1) {
                continue;
            }
        }

        QuinticHermiteSpline spline = splines[idx];
        curvePen.setColor(spline.color);

        drawControlPoints(idx, &spline.color, &curvePen, ignore);



        for (int i = 0; i <= points; i++) {
            double t = (double(i) / double(points));
            double x = spline.evaluatePoint(t, &QuinticHermiteSpline::basisFunctions, true);
            double y = spline.evaluatePoint(t, &QuinticHermiteSpline::basisFunctions, false);

            y = double(height) / 2 - y * scaleY;

            x = double(width) / 2 + x * (scaleX);


            if (i == 0 || y < 0 || y > height){
                prevPoint = QPointF(x, y);
                continue;
            }
            QPointF point = QPointF(x, y);
            auto *curve = new QGraphicsLineItem(QLineF(prevPoint,point));
            // get global position of curve

            curve->setPen(curvePen);
            curve->setData(0, idx);
            scene->addItem(curve);
            prevPoint = point;
        }
    }

}

void Path::drawControlPoints(int splineIdx, QColor *color, QPen *curvePen, int ignore) {
    if (splines.isEmpty()){
        return;
    }
    if (splineIdx == 0) {
        drawStartControlPoints(color, curvePen, ignore);
    }
    if (splineIdx == splines.size() - 1) {
        drawStartControlPoints(color, curvePen, ignore, false);
        drawMiddleControlPoints(splineIdx, color, curvePen, ignore);
    }

    if (splineIdx != 0 && splineIdx != splines.size() - 1) {
        drawMiddleControlPoints(splineIdx, color, curvePen, ignore);
    }
}

void Path::drawStartControlPoints(QColor *color, QPen *curvePen, int ignore, bool start){
    // Draw a circle to mark the start of a spline
    QPen *curvePen2 = new QPen();
    curvePen2->setColor(Qt::gray);
    curvePen2->setStyle(Qt::DashLine);
    curvePen2->setWidthF(2.0);
    int idx;
    if (start){
        idx = 0;
    } else {
        idx = splines.size() - 1;
    }

    QGraphicsScene *scene = parent->graphicsScene_;
    double x, y, xV, yV, xA, yA;
    if (start){
        x = splines[idx].start.x();
        y = splines[idx].start.y();
    } else {
        x = splines[idx].end.x();
        y = splines[idx].end.y();
    }
    double width = scene->width();
    double height = scene->height();

    double ystart = height / 2 - y * scaleY;
    double xstart = width / 2 + x * scaleX;

    if (start) {
        xA = splines[idx].startAcceleration.x();
        yA = splines[idx].startAcceleration.y();
    } else {
        xA = splines[idx].endAcceleration.x();
        yA = splines[idx].endAcceleration.y();
    }

    xA = width / 2 + (x + xA) * scaleX;
    yA = height / 2 - (y + yA) * scaleY;

    if (start) {
        xV = splines[idx].startVelocity.x();
        yV = splines[idx].startVelocity.y();
    } else {
        xV = splines[idx].endVelocity.x();
        yV = splines[idx].endVelocity.y();
    }

    xV = width / 2 + (x + xV) * scaleX;
    yV = height / 2 - (y + yV) * scaleY;

    QBrush brush(Qt::SolidPattern);
    brush.setColor(*color);


    if ((ignore != 0 && start) || (ignore != 1 && !start)) {
        int type = 0 + 1*(!start);

        auto *circle = new ControlPointUI(type, idx, this);
        circle->setRect(0, 0, 10, 10);
        circle->setPos(xstart - 5, ystart - 5);

        circle->setBrush(brush);
        circle->setPen(*curvePen);
        circle->setZValue(500);
        circle->setOpacity(0.9);
        circle->setParent(scene);
        scene->addItem(circle);
    }

    if ((ignore != 2 && start) || (ignore != 4 && !start)){
        int type = 2 + 2*(!start);
        auto *circleV = new ControlPointUI(type, idx, this);
        circleV->setRect(0, 0, 10, 10);
        circleV->setPos(xV - 5, yV - 5);
        circleV->setBrush(brush);
        circleV->setPen(*curvePen);
        circleV->setZValue(500);
        circleV->setOpacity(0.9);
        circleV->setParent(scene);
        scene->addItem(circleV);
    }

    if ((ignore != 3 && start) || (ignore != 5 && !start)) {
        int type = 3 + 2*(!start);
        auto *circleA = new ControlPointUI(type, idx, this);
        circleA->setRect(0, 0, 10, 10);
        circleA->setPos(xA - 5, yA - 5);
        circleA->setBrush(brush);
        circleA->setPen(*curvePen);
        circleA->setZValue(500);
        circleA->setOpacity(0.9);
        circleA->setParent(scene);
        scene->addItem(circleA);
    }

    auto *line = new QGraphicsLineItem(QLineF(QPointF(xstart, ystart), QPointF(xA, yA)));
    line->setPen(*curvePen2);
    QGraphicsTextItem *text = scene->addText("A");
    text->setPos((line->line().p1().x() + line->line().p2().x() - text->boundingRect().width()) / 2,
                 line->line().p2().y() + 5);
    text->setData(0, idx);
    line->setData(0, idx);
    scene->addItem(line);

    auto *line2 = new QGraphicsLineItem(QLineF(QPointF(xstart, ystart), QPointF(xV, yV)));
    text = scene->addText("V");
    text->setPos((line2->line().p1().x() + line2->line().p2().x() - text->boundingRect().width()) / 2,
                 line2->line().p2().y() + 5);
    line2->setPen(*curvePen2);
    line2->setData(0, idx);
    text->setData(0, idx);
    scene->addItem(line2);
}

void Path::drawMiddleControlPoints(int splineIdx, QColor *color, QPen *curvePen, int ignore) {
    if (splines.isEmpty() || splineIdx == 0){
        return;
    }
    QPen *curvePen2 = new QPen();
    curvePen2->setColor(Qt::gray);
    curvePen2->setStyle(Qt::DashLine);
    curvePen2->setWidthF(2.0);

    QGraphicsScene *scene = parent->graphicsScene_;
    double x, y, xV, yV, xA, yA;

    x = splines[splineIdx].start.x();
    y = splines[splineIdx].start.y();
    double width = scene->width();
    double height = scene->height();

    double ystart = height / 2 - y * scaleY;
    double xstart = width / 2 + x * scaleX;

    xA = splines[splineIdx].startAcceleration.x();
    yA = splines[splineIdx].startAcceleration.y();

    xA = width / 2 + (x + xA) * scaleX;
    yA = height / 2 - (y + yA) * scaleY;

    xV = splines[splineIdx].startVelocity.x();
    yV = splines[splineIdx].startVelocity.y();

    xV = width / 2 + (x + xV) * scaleX;
    yV = height / 2 - (y + yV) * scaleY;

    QBrush brush(Qt::SolidPattern);
    brush.setColor(*color);

    if (ignore != 6){
        int type = 6;
        auto *circle = new ControlPointUI(type, splineIdx, this);
        circle->setRect(0, 0, 10, 10);
        circle->setPos(xstart - 5, ystart - 5);
        circle->setBrush(brush);
        circle->setPen(*curvePen);
        circle->setZValue(500);
        circle->setOpacity(0.9);
        circle->setParent(scene);
        scene->addItem(circle);
    }

    if (ignore != 7){
        int type = 7;
        auto *circleV = new ControlPointUI(type, splineIdx, this);
        circleV->setRect(0, 0, 10, 10);
        circleV->setPos(xV - 5, yV - 5);
        circleV->setBrush(brush);
        circleV->setPen(*curvePen);
        circleV->setZValue(500);
        circleV->setOpacity(0.9);
        circleV->setParent(scene);
        scene->addItem(circleV);
    }

    if (ignore != 8){
        int type = 8;
        auto *circleA = new ControlPointUI(type, splineIdx, this);
        circleA->setRect(0, 0, 10, 10);
        circleA->setPos(xA - 5, yA - 5);
        circleA->setBrush(brush);
        circleA->setPen(*curvePen);
        circleA->setZValue(500);
        circleA->setOpacity(0.9);
        circleA->setParent(scene);
        scene->addItem(circleA);
    }


    auto *line = new QGraphicsLineItem(QLineF(QPointF(xstart, ystart), QPointF(xA, yA)));
    line->setPen(*curvePen2);
    QGraphicsTextItem *text = scene->addText("A");
    text->setPos((line->line().p1().x() + line->line().p2().x() - text->boundingRect().width()) / 2,
                 line->line().p2().y() + 5);
    text->setData(0, splineIdx);
    line->setData(0, splineIdx);
    scene->addItem(line);

    auto *line2 = new QGraphicsLineItem(QLineF(QPointF(xstart, ystart), QPointF(xV, yV)));
    text = scene->addText("V");
    text->setPos((line2->line().p1().x() + line2->line().p2().x() - text->boundingRect().width()) / 2,
                 line2->line().p2().y() + 5);
    line2->setPen(*curvePen2);
    line2->setData(0, splineIdx);
    text->setData(0, splineIdx);
    scene->addItem(line2);

}

void Path::animate(){
    // animate an object moving along the path

    QGraphicsScene *scene = parent->graphicsScene_;
    double width = scene->width();
    double height = scene->height();
    QBrush brush(Qt::SolidPattern);
    brush.setColor(Qt::red);
    double y = splines[0].start.y();
    double x = splines[0].start.x();
    double ystart = height / 2 - y * scaleY;
    double xstart = width / 2 + x * scaleX;
    double yend = height / 2 - splines[splines.size()-1].end.y() * scaleY;
    double xend = width / 2 + splines[splines.size()-1].end.x() * scaleX;

    // Create object
    auto *circle = new AnimatedObject();
    circle->setRect(0, 0, 20, 20);
    circle->setPos(xstart, ystart);
    circle->setZValue(2000);
    circle->setBrush(brush);
    scene->addItem(circle);


    // Animate object

    auto *animation = new QPropertyAnimation(circle, "pos");
    animation->setDuration(1000*splines.size());
    animation->setStartValue(QPointF(xstart, ystart));
    animation->setEndValue(QPointF(xend, yend));

    int j = 0;
    for (const QuinticHermiteSpline& spline: splines){

        for (int i = 0; i <= 1000; i++) {
            double t = i / double(1000);
            double xp = spline.evaluatePoint(t, &QuinticHermiteSpline::basisFunctions, true);
            double yp = spline.evaluatePoint(t, &QuinticHermiteSpline::basisFunctions, false);

            yp = double(height) / 2 - yp * scaleY;

            xp = double(width) / 2 + xp * (scaleX);

            animation->setKeyValueAt(double(i+j*1000)/(1000*splines.size()), QPointF(xp, yp));
        }
        j++;
    }
    QObject::connect(animation, &QPropertyAnimation::finished, [animation]() {
        delete animation->targetObject();
    });
    animation->start();

}
