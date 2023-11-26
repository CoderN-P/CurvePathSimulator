#pragma once
#include <QtSerialPort/QSerialPort>
#include "RobotPos.h"
#include <QPointF>

class ArduinoClient {
    QSerialPort *serialPort;
public:
    ArduinoClient(QSerialPort *serialPort);
    RobotPos getPosition();
};


