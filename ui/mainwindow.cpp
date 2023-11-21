#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsScene>
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

    zoomSlider_ = new QSlider(Qt::Horizontal, this);
    zoomSlider_->setGeometry(QRect(QPoint(100, 1000),
                                   QSize(400, 50)));
    zoomSlider_->setRange(1, 10);


    panSlider_ = new QSlider(Qt::Horizontal, this);
    panSlider_->setGeometry(QRect(QPoint(100, 1100),
                                   QSize(400, 50)));
    panSlider_->setRange(0, 1);


    spline_ = new QuinticHermiteSpline(1, 3, 2, 3, 1, 2, 200, 50);

    auto *layout = new QGridLayout;

    layout->addWidget(button_, 3, 0, 1, 2);
    layout->addWidget(portLabel_, 0, 0, 1, 1);
    layout->addWidget(comPort_, 0, 1, 1, 1);
    layout->addWidget(zoomSlider_, 1, 0, 1, 1);
    layout->addWidget(graphicsView_, 2, 0, 1, 2);
    layout->addWidget(panSlider_, 1, 1, 1, 1);

    connect(button_, SIGNAL(released()), this, SLOT(sendToRobot()));
    connect(zoomSlider_, SIGNAL(valueChanged(int)), this, SLOT(updateZoom()));
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
    delete spline_;
    delete zoomSlider_;
    delete panSlider_;
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
    spline_->draw(graphicsView_, graphicsScene_, 100, -2, 2);
    this->drawAxis();
}

void MainWindow::drawAxis() const{
    int graphicsViewWidth = graphicsView_->width();
    int graphicsViewHeight = graphicsView_->height();
    graphicsScene_->addLine((int)(graphicsViewWidth/2), 0, (int)(graphicsViewWidth/2), graphicsViewHeight);
    graphicsScene_->addLine(0, (int)(graphicsViewHeight/2), graphicsViewWidth, (int)(graphicsViewHeight/2));

}

void MainWindow::updateZoom() {
    graphicsScene_->clear();
    spline_->draw(graphicsView_, graphicsScene_, 100, -2, 2, zoomSlider_->value(), panSlider_->value());
    this->drawAxis();
}