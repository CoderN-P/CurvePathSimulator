#pragma once
#include <QWidget>
#include <QProcess>


class QComboBox;
class QPushButton;
class QLabel;
class QGraphicsView;
class QGraphicsScene;
class QuinticHermiteSpline;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    // constructor
    explicit MainWindow(QWidget *parent = nullptr);
    // destructor
    ~MainWindow() override;

private slots:
    void sendToRobot();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QPushButton* button_{};
    QProcess process_{};
    QComboBox* comPort_{};
    QLabel* portLabel_{};

public:
    QGraphicsView* graphicsView_{};
    QGraphicsScene* graphicsScene_{};
    QuinticHermiteSpline* spline_{};

};

