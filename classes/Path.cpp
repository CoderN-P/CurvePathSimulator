#include <QGraphicsTextItem>
#include "Path.h"


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
    int start = splines[0].start;
    int end = splines.last().end;

    int diff = end - start;

    double scaleX;
    // Check if startPoint or endPoint is greater in magnitude
    // The one with the greater magnitude will be used to determine how many x ticks are needed
    if (fabs(start) > fabs(end)){
        scaleX = (width) / (zoom * 2 * fabs(start));
    } else {
        scaleX = (width) / (zoom * 2 * fabs(end));
    }

    double scaleY = scaleX * (double(height) / width);

    // Draw ticks and labels
    drawGridLines(scaleX, scaleY, width, height, scene);
    drawTicksAndLabels(scaleX, scaleY, width, height, scene);

    double prevPoint[2];
    for (int idx = 0; idx < splines.size(); idx++) {
        QuinticHermiteSpline spline = splines[idx];
        for (int i = 0; i <= points; i++) {
            double t = (diff * (double(i) / double(points)) + start);
            double y = spline.evaluatePoint(t, &spline.basisFunctions);

            y = double(height) / 2 - y * scaleY;
            t = double(width) / 2 + t * (scaleX);

            if (i == 0 || y > height || y < 0) {
                prevPoint[0] = t;
                prevPoint[1] = y;
                continue;
            }

            scene->addLine(prevPoint[0], prevPoint[1], t, y);
            prevPoint[0] = t;
            prevPoint[1] = y;
        }
    }
}