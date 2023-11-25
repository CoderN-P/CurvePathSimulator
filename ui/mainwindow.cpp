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
    setMouseTracking(true);

    button_ = new QPushButton(tr("Send to robot"), this);
    button_->setGeometry(QRect(QPoint(100, 100),
                               QSize(400, 50)));

    button_->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"  // Blue background color
            "   color: #ffffff;"             // White text color
            "   padding: 10px 20px;"          // Padding for the button
            "   border: none;"               // No border
            "   border-radius: 5px;"          // Rounded corners
            "   font-size: 16px;"            // Font size
            "   font-weight: bold;"           // Bold text
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"  // Darker blue on hover
            "}"
    );


    portLabel_ = new QLabel(tr("Port: "), this);
    portLabel_->setGeometry(QRect(QPoint(10, 10),
                                  QSize(100, 50)));
    comPort_ = new QComboBox(this);
    comPort_->setGeometry(QRect(QPoint(50, 10),
                                QSize(300, 50)));

    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        comPort_->addItem(info.portName());
    }

    plusButton_ = new QPushButton("Add Spline");

    // Apply a minimalistic Tailwind-inspired stylesheet to the QPushButton
    plusButton_->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"    // Background color of the button
            "   color: #ffffff;"               // Text color of the button
            "   padding: 10px;"                // Padding of the button
            "   border: none;"                 // No border for the button
            "   font-size: 16px;"
            "   border-radius: 5px;"// Font size of the text
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"    // Background color on hover
            "}"
    );


    graphicsView_ = new QGraphicsView(this);
    graphicsView_->setGeometry(QRect(QPoint(100, 200),
                                     QSize(400, 800)));

    graphicsScene_ = new QGraphicsScene();
    graphicsView_->setScene(graphicsScene_);


    zoomSlider_ = new QSlider(Qt::Horizontal, this);
    zoomSlider_->setGeometry(QRect(QPoint(100, 1000),
                                   QSize(400, 50)));
    zoomSlider_->setRange(1, 10);
    zoomSlider_->setValue(2);

    zoomSlider_->setStyleSheet(
            "QSlider::groove:horizontal {"
            "   border: 1px solid #bdc3c7;"      // Border color of the groove
            "   height: 10px;"                   // Height of the groove
            "   background: #ecf0f1;"            // Background color of the groove
            "   margin: 0px;"
            "   border-radius: 5px;"
            "}"
            "QSlider::handle:horizontal {"
            "   background: #3498db;"            // Handle color
            "   border: 1px solid #2980b9;"      // Border color of the handle
            "   width: 20px;"                    // Width of the handle
            "   height: 20px;"                   // Height of the handle
            "   margin: -5px 0px;"               // Positioning of the handle
            "   border-radius: 10px;"            // Rounded corners
            "}"
            "QSlider::sub-page:horizontal {"
            "   background: #3498db;"            // Color of the filled portion of the groove
            "}"
            "QSlider::add-page:horizontal {"
            "   background: #ecf0f1;"            // Color of the unfilled portion of the groove
            "}"
    );



    zoomLabel_ = new QLabel(tr("Zoom: "), this);
    zoomLabel_->setGeometry(QRect(QPoint(10, 1000),
                                  QSize(100, 50)));

    zoomLabel_->setStyleSheet(
            "QLabel {"
            "   font-size: 16px;"            // Font size
            "   font-weight: bold;"           // Bold text
            "}"
    );

    panLabel_ = new QLabel(tr("Pan: "), this);
    panLabel_->setGeometry(QRect(QPoint(10, 1100),
                                  QSize(100, 50)));

    panLabel_->setStyleSheet(
            "QLabel {"
            "   font-size: 16px;"            // Font size
            "   font-weight: bold;"
            "   color: #FFFFFF"// Bold text
            "}"
    );


    panSlider_ = new QSlider(Qt::Horizontal, this);
    panSlider_->setGeometry(QRect(QPoint(100, 1100),
                                   QSize(400, 50)));
    panSlider_->setRange(0, 1);

    animateButton_ = new QPushButton(tr("Animate"), this);
    animateButton_->setGeometry(QRect(QPoint(100, 1200),
                               QSize(400, 50)));
    animateButton_->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"  // Blue background color
            "   color: #ffffff;"             // White text color
            "   padding: 10px 20px;"          // Padding for the button
            "   border: none;"               // No border
            "   border-radius: 5px;"          // Rounded corners
            "   font-size: 16px;"            // Font size
            "   font-weight: bold;"           // Bold text
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"  // Darker blue on hover
            "}"
    );
    auto *spline = new QuinticHermiteSpline(QPointF(-2, -2), QPointF(-1, 0), QPointF(0, 1), QPointF(3, 2), QPointF(1, 1), QPointF(0, 1));

    graphicsView_->setRenderHint(QPainter::Antialiasing, true);

    QVector<QuinticHermiteSpline> splineList = {*spline};

    path_ = new Path(splineList, this);

    auto *layout = new QGridLayout;

    layout->addWidget(button_, 4, 0, 1, 4);
    layout->addWidget(plusButton_, 0, 3, 1, 1);
    layout->addWidget(portLabel_, 0, 0, 1, 1);
    layout->addWidget(comPort_, 0, 1, 1, 1);
    layout->addWidget(zoomSlider_, 1, 1, 1, 1);
    layout->addWidget(zoomLabel_, 1, 0, 1, 1);
    layout->addWidget(graphicsView_, 3, 0, 1, 4);
    layout->addWidget(panSlider_, 1, 3, 1, 1);
    layout->addWidget(panLabel_, 1, 2, 1, 1);
    layout->addWidget(animateButton_, 2, 0, 1, 4);

    connect(button_, SIGNAL(released()), this, SLOT(sendToRobot()));
    connect(zoomSlider_, SIGNAL(valueChanged(int)), this, SLOT(updateZoom()));
    connect(plusButton_, SIGNAL(released()), this, SLOT(addSpline()));
    connect(animateButton_, SIGNAL(released()), this, SLOT(animate()));
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
    delete path_;
    delete plusButton_;
    delete animateButton_;

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
    graphicsScene_->setSceneRect(0, 0, graphicsView_->width(), graphicsView_->height());
    graphicsScene_->clear();
    path_->draw(graphicsView_, graphicsScene_, zoomSlider_->value(), 1000);
}

void MainWindow::drawAxis() const{
    int graphicsViewWidth = graphicsView_->width();
    int graphicsViewHeight = graphicsView_->height();

    graphicsScene_->addLine((int)(graphicsViewWidth/2), 0, (int)(graphicsViewWidth/2), graphicsViewHeight);
    graphicsScene_->addLine(0, (int)(graphicsViewHeight/2), graphicsViewWidth, (int)(graphicsViewHeight/2));

}

void MainWindow::updateZoom() const {
    graphicsScene_->clear();
    path_->draw(graphicsView_, graphicsScene_, zoomSlider_->value(), 1000);
}

void MainWindow::addSpline() {
    QuinticHermiteSpline prevSpline = path_->splines[path_->splines.size()-1];
    auto *spline = new QuinticHermiteSpline(prevSpline.end, QPointF(prevSpline.end.x()+2, prevSpline.end.y()+2), prevSpline.endVelocity, QPointF(0, 1), prevSpline.endAcceleration, QPointF(0, 1));
    path_->splines.append(*spline);
    graphicsScene_->clear();
    path_->draw(graphicsView_, graphicsScene_, zoomSlider_->value(), 1000);
}

void MainWindow::animate() const {
    path_->animate();
}


