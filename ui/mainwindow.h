#pragma once
#include <QWidget>
#include <QProcess>


class QComboBox;
class QPushButton;
class QLabel;
class QGraphicsView;
class QGraphicsScene;
class QuinticHermiteSpline;
class QSlider;
class QLineEdit;


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
    void updateZoom();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawAxis() const;
    QPushButton* button_{};
    QProcess process_{};
    QComboBox* comPort_{};
    QLabel* portLabel_{};
    QSlider* zoomSlider_{};
    QSlider* panSlider_{};
    QLineEdit* startRange_{};
    QLineEdit* endRange_{};
    QLabel* zoomLabel_{};
    QLabel* panLabel_{};
    QLabel* startRangeLabel_{};
    QLabel* endRangeLabel_{};
    QSlider* resolution_{};
    QLabel* resolutionLabel_{};


public:
    QGraphicsView* graphicsView_{};
    QGraphicsScene* graphicsScene_{};
    QuinticHermiteSpline* spline_{};

};

