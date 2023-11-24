//
// Created by GreatNeel on 11/22/23.
//

#include "ControlPointUI.h"
#include "Path.h"
#include "QuinticHermiteSpline.h"
#include <QSlider>
#include <iostream>

QVariant ControlPointUI::itemChange(GraphicsItemChange change, const QVariant &value) {


    if (change == 0 && scene()) {
        if (ptype == 0) {
            double scaleX = parentPath->scaleX;
            double scaleY = parentPath->scaleY;
            int width = parentPath->parent->graphicsScene_->width();
            int height = parentPath->parent->graphicsScene_->height();
            // convert newPos to actual coordinates
            // Map to coordinates in graphicsView
            QPointF newPos = scenePos();
            double y = (newPos.y() - double(height)/2)/-scaleY;
            double x = (newPos.x() - double(width)/2)/scaleX;

            QPointF convertedPos = QPointF(x, y);

            parentPath->splines[splineIdx].start = convertedPos;
        }
        // Remove all items except the control points
        for (QGraphicsItem *item : scene()->items()) {
            if (item->type() != type()) {
                scene()->removeItem(item);
            }
        }


        parentPath->draw(parentPath->parent->graphicsView_, parentPath->parent->graphicsScene_, parentPath->parent->zoomSlider_->value(), 100, false, false);

        // Call the base class implementation to allow the item to move
        return QGraphicsEllipseItem::itemChange(change, value);
    }

    // For other changes, call the base class implementation
    return QGraphicsEllipseItem::itemChange(change, value);
}
