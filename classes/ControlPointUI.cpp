//
// Created by GreatNeel on 11/22/23.
//

#include "ControlPointUI.h"
#include "Path.h"
#include "QuinticHermiteSpline.h"
#include <QSlider>
#include <iostream>

QVariant ControlPointUI::itemChange(GraphicsItemChange change, const QVariant &value) {
    double scaleX = parentPath->scaleX;
    double scaleY = parentPath->scaleY;
    int width = parentPath->parent->graphicsScene_->width();
    int height = parentPath->parent->graphicsScene_->height();

    QPointF newPos = scenePos();
    double y = (newPos.y() - double(height)/2)/-scaleY;
    double x = (newPos.x() - double(width)/2)/scaleX;

    QPointF convertedPos = QPointF(x, y);

    if (change == 0 && scene()) {
        if (ptype == 0) {
            parentPath->splines[splineIdx].start = convertedPos;
        } else if (ptype == 1) {
            parentPath->splines[splineIdx].end = convertedPos;
        } else if (ptype == 2) {
            QuinticHermiteSpline *spline = &parentPath->splines[splineIdx];
            parentPath->splines[splineIdx].startVelocity = QPointF(convertedPos.x() - spline->start.x(), convertedPos.y() - spline->start.y());
        } else if (ptype == 3) {
            QuinticHermiteSpline *spline = &parentPath->splines[splineIdx];
            parentPath->splines[splineIdx].startAcceleration = QPointF(convertedPos.x()-spline->start.x(), convertedPos.y()-spline->start.y());
        } else if (ptype == 4) {
            QuinticHermiteSpline *spline = &parentPath->splines[splineIdx];
            parentPath->splines[splineIdx].endVelocity = QPointF(convertedPos.x()-spline->end.x(), convertedPos.y()-spline->end.y());
        } else if (ptype == 5) {
            QuinticHermiteSpline *spline = &parentPath->splines[splineIdx];
            parentPath->splines[splineIdx].endAcceleration = QPointF(convertedPos.x()-spline->end.x(), convertedPos.y()-spline->end.y());
        } else if (ptype == 6){
            parentPath->splines[splineIdx].start = convertedPos;
            parentPath->splines[splineIdx-1].end = convertedPos;
        } else if (ptype == 7){
            QuinticHermiteSpline *spline = &parentPath->splines[splineIdx];
            parentPath->splines[splineIdx].startVelocity = QPointF(convertedPos.x() - spline->start.x(), convertedPos.y() - spline->start.y());
            parentPath->splines[splineIdx-1].endVelocity = QPointF(convertedPos.x() - spline->start.x(), convertedPos.y() - spline->start.y());
        } else if (ptype == 8){
            QuinticHermiteSpline *spline = &parentPath->splines[splineIdx];
            parentPath->splines[splineIdx].startAcceleration = QPointF(convertedPos.x()-spline->start.x(), convertedPos.y()-spline->start.y());
            parentPath->splines[splineIdx-1].endAcceleration = QPointF(convertedPos.x()-spline->start.x(), convertedPos.y()-spline->start.y());
        }

        // Remove all items except the control points
        for (QGraphicsItem *item : scene()->items()) {
            if (item->type() != type()) {
                scene()->removeItem(item);
            } else {
                auto *controlPoint = dynamic_cast<ControlPointUI *>(item);
                if (controlPoint->ptype != ptype) {
                    scene()->removeItem(item);
                }
            }
        }

        int ignoreIdx = -1;
        int ignoreIdx2 = -1;
        if (ptype < 6){
            ignoreIdx = splineIdx;
        } else {
            ignoreIdx = splineIdx-1;
            ignoreIdx2 = splineIdx;
        }
        parentPath->draw(parentPath->parent->graphicsView_, parentPath->parent->graphicsScene_, parentPath->parent->zoomSlider_->value(), 100, ptype, false, ignoreIdx, ignoreIdx2);

        // Call the base class implementation to allow the item to move
        return QGraphicsEllipseItem::itemChange(change, value);
    }

    // For other changes, call the base class implementation
    return QGraphicsEllipseItem::itemChange(change, value);
}


