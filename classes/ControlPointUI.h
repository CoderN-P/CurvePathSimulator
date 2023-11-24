//
// Created by GreatNeel on 11/22/23.
//
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QObject>
#include <QBrush>
#include <iostream>
#pragma once

class QuinticHermiteSpline;
class Path;

class ControlPointUI: public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    // 0 = start, 1 = end, 2 = velocityTangentStart, 3 = accelerationTangentStart, 4 = velocityTangentEnd, 5 = accelerationTangentEnd, 6 = start/end, 7 = start/end, 8 = velocityTangentStart/velocityTangentEnd, 9 = accelerationTangentStart/accelerationTangentEnd
    int ptype;
    int splineIdx;
    Path *parentPath;
    explicit ControlPointUI(int type=0, int splineIdx=0, Path *path=nullptr)
    : QObject(), QGraphicsEllipseItem() {


        this->ptype = type;
        this->splineIdx = splineIdx;
        this->parentPath = path;

        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};


