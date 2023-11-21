#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsScene>
#include "drawAxis.cpp"
#include "../classes/QuinticHermiteSpline.h"
#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

    button_ = new QPushButton(tr("Send to robot"), this);
    button_->setGeometry(QRect(QPoint(100, 100),
                               QSize(400, 50)));


    portLabel_ = new QLabel(tr("Port: "), this);
    portLabel_->setGeometry(QRect(QPoint(10, 10),
                                  QSize(100, 50)));
    comPort_ = new QComboBox(this);
    comPort_->setGeometry(QRect(QPoint(50, 10),
                                QSize(300, 50)));

    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        comPort_->addItem(info.portName());
    }

    graphicsView_ = new QGraphicsView(this);
    graphicsView_->setGeometry(QRect(QPoint(100, 200),
                                     QSize(400, 800)));

    graphicsScene_ = new QGraphicsScene();
    graphicsView_->setScene(graphicsScene_);

    spline_ = new QuinticHermiteSpline(1, 3, 2, 3, 1, 2, 200, 50);

    auto *layout = new QGridLayout;

    layout->addWidget(button_, 2, 0, 1, 2);
    layout->addWidget(portLabel_, 0, 0, 1, 1);
    layout->addWidget(comPort_, 0, 1, 1, 1);
    layout->addWidget(graphicsView_, 1, 0, 1, 2);


    connect(button_, SIGNAL(released()), this, SLOT(sendToRobot()));
    setLayout(layout);



    setWindowTitle(tr("Robot Path Planner"));

};

MainWindow::~MainWindow()
{
    delete button_;
    delete comPort_;
    delete portLabel_;
    delete graphicsView_;
    delete graphicsScene_;
}

void MainWindow::sendToRobot() {
    if (comPort_->currentText().isEmpty()) {
        QMessageBox::warning(this, tr("No port selected"),
                             tr("Please select a port"));
        return;
    }

    QSerialPort serialPort;

    serialPort.setPortName(comPort_->currentText());



}


void MainWindow::resizeEvent(QResizeEvent *event) {

    QWidget::resizeEvent(event);
    graphicsScene_->clear();
    spline_->draw(graphicsView_, graphicsScene_, 1000, -2, 2);
    drawAxis(graphicsView_, graphicsScene_);
}