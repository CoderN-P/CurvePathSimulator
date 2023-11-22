#include <QGraphicsTextItem>
#include "Path.h"
#include <iostream>


Path::Path(QVector<QuinticHermiteSpline> splineList) {
    this->splines = splineList;
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



void Path::draw(QGraphicsView *view, QGraphicsScene *scene, double zoom, int points){
    int height = view->height();
    int width = view->width();

    if (splines.isEmpty()){
        return;
    }
    double scaleX;
    double maxX = 0;
    for (QuinticHermiteSpline spline : splines){
        maxX = std::max(maxX, std::max(fabs(spline.start.x()), fabs(spline.end.x())));
    }

    // Check if startPoint or endPoint is greater in magnitude
    // The one with the greater magnitude will be used to determine how many x ticks are needed
    scaleX = (width) / (zoom * 2 * fabs(maxX));


    double scaleY = scaleX * (double(height) / width);

    // Draw ticks and labels
    drawGridLines(scaleX, scaleY, width, height, scene);
    drawTicksAndLabels(scaleX, scaleY, width, height, scene);
    QGraphicsItemGroup *group = new QGraphicsItemGroup();
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
            double x = spline.evaluatePoint(t, &spline.basisFunctions, true);
            double y = spline.evaluatePoint(t, &spline.basisFunctions, false);

            y = double(height) / 2 - y * scaleY;

            x = double(width) / 2 + x * (scaleX);

            if (i == 0 || y < 0 || y > height){
                prevPoint = QPointF(x, y);
                if (i == 0){
                    // Draw a circle to mark the start of a spline
                    QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(QRectF(x-5, y-5, 10, 10));
                    // fill circle
                    QBrush brush(Qt::SolidPattern);
                    brush.setColor(colors[idx]);
                    circle->setBrush(brush);
                    circle->setPen(curvePen);
                    // set circle to highest z value
                    circle->setZValue(1000);
                    // lower opacity by 10%
                    circle->setOpacity(0.9);
                    group->addToGroup(circle);
                }
                continue;
            }
            QPointF point = QPointF(x, y);
            QGraphicsLineItem *curve = new QGraphicsLineItem(QLineF(prevPoint,point));
            curve->setPen(curvePen);
            group->addToGroup(curve);
            prevPoint = point;
        }
    }

    scene->addItem(group);

    // Draw a circle at the start and end of the path

}