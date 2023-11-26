#include "ArduinoClient.h"
#include <QPointF>

RobotPos ArduinoClient::getPosition() {
    serialPort->write("getPos");

}

ArduinoClient::ArduinoClient(QSerialPort *serialPort) {

}
