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
#include "../classes/PurePursuitPath.h"
#include "../classes/RobotPos.h"
#include "../classes/PurePursuit.h"
#include "../classes/Ramsete.h"


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
    auto *spline = new QuinticHermiteSpline(prevSpline.end, QPointF(prevSpline.end.x()+2, prevSpline.end.y()+2), prevSpline.endVelocity, QPointF(0, 1), prevSpline.endAcceleration, QPointF(1, 1));
    path_->splines.append(*spline);
    graphicsScene_->clear();
    path_->draw(graphicsView_, graphicsScene_, zoomSlider_->value(), 1000);
}

void MainWindow::animate() {
    animateRamsete();
}
void MainWindow::animateRamsete() {
    auto ramsete = new Ramsete(path_, 0.5, 0.7);
    auto *robotGroup = new QGraphicsItemGroup();
    double xstart = path_->splines[0].start.x();
    double ystart = path_->splines[0].start.y();
    double scaleX = path_->scaleX;
    double scaleY = path_->scaleY;
    double width = graphicsView_->width();
    double height = graphicsView_->height();
    double x = double(width) / 2 + xstart * (scaleX);
    double y = double(height) / 2 - ystart * scaleY;
    auto *robot = new QGraphicsEllipseItem(x-15, y-15, 30, 30);
    robot->setBrush(QBrush(Qt::red));
    robot->setPen(QPen(Qt::red));
    robot->setZValue(2000);
    // calculate angle of robot

    double theta = atan2(path_->splines[0].startVelocity.y(), path_->splines[0].startVelocity.x())*180/M_PI;

    auto *robotHeading = new QGraphicsLineItem(x, y, x+0.5*scaleX*cos(theta), y-0.5*scaleY*sin(theta));
    robotHeading->setZValue(2001);
    robotHeading->setPen(QPen(Qt::gray, 2));
    robotGroup->addToGroup(robot);
    robotGroup->addToGroup(robotHeading);
    graphicsScene_->addItem(robotGroup);
    auto* robotPos = new RobotPos(xstart, ystart, theta, 0, 0, 0.5);
    double dt = 50; // ms
    double curTime = 0;
    double totalTime = 2000*path_->splines.size(); // How long to run the animation for

    double speed = 2; // feet per second

    // model: 200rpm drive with 18" width
    //                  rpm   /s  circ   feet
    double maxLinVelfeet = double(200) / 60 * M_PI*4 / 12;
    //                  rpm  /s  center angle   deg
    double maxTurnVelDeg = double(200) / 60 * M_PI*4 / 9 *180/M_PI;

    robotPos->theta = robotPos->theta*180/M_PI;

    while (true){
        if (curTime > totalTime){
            break;
        }
        QVector<double> data = ramsete->run(robotPos, curTime);

        double linearVelocity = data[0];
        double angularVelocity = data[1];
        double stepDist = linearVelocity * dt/1000;
        std::cout << "theta " << robotPos->theta << " linearVelocity " << linearVelocity << " stepDist " << stepDist << std::endl;
        robotPos->setX(robotPos->x() + stepDist*cos(robotPos->theta*M_PI/180));
        robotPos->setY(robotPos->y() + stepDist*sin(robotPos->theta*M_PI/180));

        double newX = robotPos->x()*scaleX + width/2;
        double newY = -robotPos->y()*scaleY + height/2;
        robotHeading->setLine(newX, newY, newX+0.5*scaleX*cos(robotPos->theta*M_PI/180), newY-0.5*scaleY*sin(robotPos->theta*M_PI/180));

        // Update robot heading line
        // Update robot position
        robot->setRect(newX-15, newY-15, 30, 30);
        // Update robot angle
        std::cout << (angularVelocity * dt/1000)*180/M_PI << std::endl;
        robotPos->theta += (angularVelocity * dt/1000)*180/M_PI;

        /*
        robotPos->theta = fmod(robotPos->theta, 360);

        if (robotPos->theta < 0) {
            robotPos->theta += 360;
        }
        */

        curTime += dt;

        QCoreApplication::processEvents();
        QThread::msleep(dt);
    }
}


void MainWindow::animatePurePursuit(){
    purePursuitPath_ = new PurePursuitPath(path_);
    purePursuitPath_->generateWaypoints(0.33);

    double lookaheadDistance = 0.5;
    double linearVelocity = 50;
    double dt = 50; // ms
    auto *robotGroup = new QGraphicsItemGroup();
    double xstart = path_->splines[0].start.x();
    double ystart = path_->splines[0].start.y();
    double scaleX = path_->scaleX;
    double scaleY = path_->scaleY;
    double width = graphicsView_->width();
    double height = graphicsView_->height();
    double x = double(width) / 2 + xstart * (scaleX);
    double y = double(height) / 2 - ystart * scaleY;
    auto *robot = new QGraphicsEllipseItem(x-15, y-15, 30, 30);
    robot->setBrush(QBrush(Qt::red));
    robot->setPen(QPen(Qt::red));
    robot->setZValue(2000);
    // calculate angle of robot

    double theta = atan2(path_->splines[0].startVelocity.y(), path_->splines[0].startVelocity.x());

    auto *robotHeading = new QGraphicsLineItem(x, y, x+lookaheadDistance*scaleX*cos(theta), y-lookaheadDistance*scaleY*sin(theta));
    robotHeading->setZValue(2001);
    robotHeading->setPen(QPen(Qt::gray, 2));
    robotGroup->addToGroup(robot);
    robotGroup->addToGroup(robotHeading);
    graphicsScene_->addItem(robotGroup);

    auto* serialPort_ = new QSerialPort(comPort_->currentText());
    auto* arduinoClient = new ArduinoClient(serialPort_);

    auto* purePursuit = new PurePursuit(lookaheadDistance, 1, 1, purePursuitPath_, arduinoClient);

    auto* robotPos = new RobotPos(xstart, ystart, theta*180/M_PI, 0, 0, 0.5);

    // Control loop
    int lastFoundIndex = 0;

    // model: 200rpm drive with 18" width
    //                  rpm   /s  circ   feet
    double maxLinVelfeet = double(200) / 60 * M_PI*4 / 12;
    //                  rpm  /s  center angle   deg
    double maxTurnVelDeg = double(200) / 60 * M_PI*4 / 9 *180/M_PI;

    // Create look ahead circle
    auto* circle = new QGraphicsEllipseItem(x-lookaheadDistance*scaleX, y-lookaheadDistance*scaleY, 2*lookaheadDistance*scaleX, 2*lookaheadDistance*scaleY);
    circle->setPen(QPen(Qt::gray, 2, Qt::DashLine));
    graphicsScene_->addItem(circle);

    auto *goalPointCircle = new QGraphicsEllipseItem(0, 0, 10, 10);
    goalPointCircle->setBrush(QBrush(Qt::green));
    graphicsScene_->addItem(goalPointCircle);

    // TODO: Fix bug where robot loops in circles at a random point in the path
    while (true) {

        if (lastFoundIndex >= purePursuitPath_->waypoints.size()-2) {
            break;
        }

        QVector<double> data = purePursuit->run(robotPos, lastFoundIndex);
        QPointF goalPoint = QPointF(data[0], data[1]);
        double turnVel = data[2];
        lastFoundIndex = int(data[3]);

        double stepDist = linearVelocity/100 * maxLinVelfeet * dt/1000;
        std::cout << "turnVel: " << turnVel << " stepDist" << stepDist << std::endl;
        robotPos->setX(robotPos->x() + stepDist*cos(robotPos->theta*M_PI/180));
        robotPos->setY(robotPos->y() + stepDist*sin(robotPos->theta*M_PI/180));

        double newX = robotPos->x()*scaleX + width/2;
        double newY = -robotPos->y()*scaleY + height/2;


        // Update robot heading line
        robotHeading->setLine(newX, newY, newX+lookaheadDistance*scaleX*cos(robotPos->theta*M_PI/180), newY-lookaheadDistance*scaleY*sin(robotPos->theta*M_PI/180));
        // Update robot position
        robot->setRect(newX-15, newY-15, 30, 30);
        // Update lookahead circle
        circle->setRect(newX-lookaheadDistance*scaleX, newY-lookaheadDistance*scaleY, 2*lookaheadDistance*scaleY, 2*lookaheadDistance*scaleX);

        // Update goal point circle
        goalPointCircle->setRect(goalPoint.x()*scaleX + width/2 - 5, -goalPoint.y()*scaleX + height/2 - 5, 10, 10);

        // Update robot angle
        robotPos->theta += (turnVel/100 * maxTurnVelDeg * dt/1000);
        robotPos->theta = fmod(robotPos->theta, 360);
        if (robotPos->theta < 0) {
            robotPos->theta += 360;
        }

        QCoreApplication::processEvents();
        QThread::msleep(dt);

    }
}
