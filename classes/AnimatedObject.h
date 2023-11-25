//
// Created by GreatNeel on 11/24/23.
//

#pragma once
#include <QObject>
#include <QGraphicsEllipseItem>

class AnimatedObject: public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
};



