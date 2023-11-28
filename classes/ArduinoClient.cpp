#include "ArduinoClient.h"
#include "RobotPos.h"


RobotPos ArduinoClient::getPosition() {
    serialPort->write("0");
    serialPort->waitForReadyRead(100);
    QByteArray data = serialPort->readAll();
    QString str(data);
    QStringList list = str.split(" ");
    double x = list[0].toDouble();
    double y = list[1].toDouble();
    double theta = list[2].toDouble();
    double velocity = list[3].toDouble();
    double angularVelocity = list[4].toDouble();
    double L = 0.5;

    return {x, y, theta, velocity, angularVelocity, L};
}

void ArduinoClient::setVelocity(double left, double right) {
    QString str = "1 " + QString::number(left) + " " + QString::number(right);
    serialPort->write(str.toStdString().c_str());
}

ArduinoClient::ArduinoClient(QSerialPort *serialPort) {
    this->serialPort = serialPort;
}
