#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <iostream>
#include "../classes/QuinticHermiteSpline.h"
#include "../classes/Path.h"
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
    zoomSlider_->setValue(6);

    zoomLabel_ = new QLabel(tr("Zoom: "), this);
    zoomLabel_->setGeometry(QRect(QPoint(10, 1000),
                                  QSize(100, 50)));
    panLabel_ = new QLabel(tr("Pan: "), this);
    panLabel_->setGeometry(QRect(QPoint(10, 1100),
                                  QSize(100, 50)));


    panSlider_ = new QSlider(Qt::Horizontal, this);
    panSlider_->setGeometry(QRect(QPoint(100, 1100),
                                   QSize(400, 50)));
    panSlider_->setRange(0, 1);

    startRangeLabel_ = new QLabel(tr("Start: "), this);
    startRangeLabel_->setGeometry(QRect(QPoint(10, 1200),
                                  QSize(100, 50)));
    endRangeLabel_ = new QLabel(tr("End: "), this);
    endRangeLabel_->setGeometry(QRect(QPoint(10, 1300),
                                  QSize(100, 50)));
    endRange_ = new QLineEdit(this);
    endRange_->setGeometry(QRect(QPoint(100, 1300),
                                  QSize(400, 50)));
    startRange_ = new QLineEdit(this);
    startRange_->setGeometry(QRect(QPoint(100, 1200),
                                  QSize(400, 50)));
    startRange_->setText("-1");
    endRange_->setText("1");

    resolution_ = new QSlider(Qt::Horizontal, this);
    resolution_->setGeometry(QRect(QPoint(100, 1400),
                                   QSize(400, 50)));
    resolution_->setRange(1, 1000);

    resolutionLabel_ = new QLabel(tr("Resolution: "), this);
    resolutionLabel_->setGeometry(QRect(QPoint(10, 1400),
                                  QSize(100, 50)));

    auto *spline = new QuinticHermiteSpline(QPoint(-2, -2), QPoint(-1, 0), QPoint(0, 1), QPoint(3, 0), QPoint(1, 1), QPoint(0, 1));
    auto *spline2 = new QuinticHermiteSpline(QPoint(-1, 0), QPoint(2, 2), QPoint(3, 0), QPoint(2, 1), QPoint(0, 1), QPoint(1, 1));
    auto *spline3 = new QuinticHermiteSpline(QPoint(2, 2), QPoint(-2, -2), QPoint(2, 1), QPoint(0, 1), QPoint(1, 1), QPoint(1, 1));

    graphicsView_->setRenderHint(QPainter::Antialiasing, true);

    QVector<QuinticHermiteSpline> splineList = {*spline, *spline2, *spline3};

    path_ = new Path(splineList);

    auto *layout = new QGridLayout;

    layout->addWidget(button_, 4, 0, 1, 4);
    layout->addWidget(portLabel_, 0, 0, 1, 1);
    layout->addWidget(resolutionLabel_, 0, 2, 1, 1);
    layout->addWidget(resolution_, 0, 3, 1, 1);
    layout->addWidget(comPort_, 0, 1, 1, 1);
    layout->addWidget(zoomSlider_, 1, 1, 1, 1);
    layout->addWidget(zoomLabel_, 1, 0, 1, 1);
    layout->addWidget(graphicsView_, 3, 0, 1, 4);
    layout->addWidget(panSlider_, 1, 3, 1, 1);
    layout->addWidget(panLabel_, 1, 2, 1, 1);
    layout->addWidget(startRange_, 2, 1, 1, 1);
    layout->addWidget(endRange_, 2, 3, 1, 1);
    layout->addWidget(startRangeLabel_, 2, 0, 1, 1);
    layout->addWidget(endRangeLabel_, 2, 2, 1, 1);

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
    delete zoomSlider_;
    delete panSlider_;
    delete zoomLabel_;
    delete panLabel_;
    delete startRange_;
    delete endRange_;
    delete startRangeLabel_;
    delete endRangeLabel_;
    delete resolution_;
    delete resolutionLabel_;
    delete path_;

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
    path_->draw(graphicsView_, graphicsScene_, zoomSlider_->value(), 1000);
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
    path_->draw(graphicsView_, graphicsScene_, zoomSlider_->value(), 1000);
    this->drawAxis();
}
