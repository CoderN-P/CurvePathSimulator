//
// Created by GreatNeel on 11/22/23.
//
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QObject>
#include <QBrush>
#pragma once

class QuinticHermiteSpline;
class Path;

class ControlPointUI: public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    int ptype; // 0 = start, 1 = end, 2 = velocityTangent, 3 = accelerationTangent
    int splineIdx;
    Path *parentPath;
    explicit ControlPointUI(int type=0, int splineIdx=0, Path *path = nullptr)
    : QObject(), QGraphicsEllipseItem() {
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

        this->ptype = type;
        this->splineIdx = splineIdx;
        this->parentPath = path;
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};


